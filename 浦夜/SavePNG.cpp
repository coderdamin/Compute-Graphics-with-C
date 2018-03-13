#include <memory.h>
#include <stdlib.h>
#include "SavePNG.h"
#include "Debug.h"

SavePNG* SavePNG::CreatePNG(char* fileName, unsigned width, unsigned height, bool alpha) {
	SavePNG* pPNG = new SavePNG;
	pPNG->m_nWidth = width;
	pPNG->m_nHeight = height;
	pPNG->m_bAlpha = alpha;
	bool bError = false;
	unsigned int nBuffSize = width * height * (alpha ? 4 : 3);
	pPNG->m_pPNGBuff = (unsigned char*)malloc(nBuffSize);
	if (pPNG->m_pPNGBuff == nullptr) {
		bError = true;
		goto ERROR_TAG;
	}
	memset(pPNG->m_pPNGBuff, 0, nBuffSize);
	if (fileName != nullptr) {
		fopen_s(&pPNG->m_fp, fileName, "wb");
		if (pPNG->m_fp == nullptr) {
			bError = true;
			goto ERROR_TAG;
		}
	}

ERROR_TAG:
	if (bError) {
		if (pPNG) {
			delete pPNG;
			pPNG = nullptr;
		}
	}
	return pPNG;
}

SavePNG::SavePNG() : m_pPNGBuff(nullptr)
, m_fp(nullptr)
, m_bAlpha(true)
, m_nWidth(0)
, m_nHeight(0)
, m_nCurCRC(0)
{}

SavePNG::~SavePNG() {
	if (m_pPNGBuff != nullptr) {
		delete m_pPNGBuff;
		m_pPNGBuff = nullptr;
	}
	if (m_fp != nullptr) {
		fclose(m_fp);
		m_fp = nullptr;
	}
}

void SavePNG::Save() {
	if (m_fp == nullptr) {
		ERROR("SavePNG", "SavePNG fp is NULL");
		return;
	}
	// ÎÄ¼þÍ·
	PutCharU8A("\x89PNG\r\n\32\n", 8);
	//IHDR
	PutCharBeginBlock("IHDR", 13);
	PutCharU32C(m_nWidth);
	PutCharU32C(m_nHeight);
	PutCharU8C(8);
	PutCharU8C(m_bAlpha ? 6 : 2);
	PutCharU8AC("\0\0\0", 3);
	PutCharEndBlock();
	//IDAT
	unsigned uPixPerRow = m_nWidth * (m_bAlpha ? 4 : 3) + 1;
	PutCharBeginBlock("IDAT", 2 + m_nHeight * ( 5 + uPixPerRow) + 4);
	PutCharU8AC("\x78\1", 2);
	unsigned char* pImageIter = m_pPNGBuff;
	for (int i = 0; i < m_nHeight; ++i) {
		PutCharU8C(i == m_nHeight - 1);
		PutCharU16LC(uPixPerRow);
		PutCharU16LC(~uPixPerRow);
		PutCharU8C(0);
		for (int j = 0; j < uPixPerRow - 1; ++j, ++pImageIter){
			PutCharU8C(*pImageIter);
		}
	}
	PutCharU32C(0);
	PutCharEndBlock();
	PutCharBeginBlock("IDAT", 0);
	PutCharEndBlock();
}

void SavePNG::PutChar(char val) {
	fputc(val, m_fp);
}

void SavePNG::PutCharU8A(char* pUA, unsigned nLen) {
	for (int i = 0; i < nLen; ++i) {
		PutChar(pUA[i]);
	}
}

void SavePNG::PutCharU32(unsigned uVal) {
	PutChar(uVal >> 24);
	PutChar((uVal >> 16) & 0xff);
	PutChar((uVal >> 8) & 0xff);
	PutChar(uVal & 0xff);
}

void SavePNG::PutCharU8C(unsigned char uVla) {
	static const unsigned t[] = { 0, 0x1db71064, 0x3b6e20c8, 0x26d930ac, 0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
		/* CRC32 Table */    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c, 0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c };
	PutChar(uVla);
	m_nCurCRC ^= uVla;
	m_nCurCRC = (m_nCurCRC >> 4) ^ t[m_nCurCRC & 15];
	m_nCurCRC = (m_nCurCRC >> 4) ^ t[m_nCurCRC & 15];
}

void SavePNG::PutCharU8AC(char* pUA, unsigned nLen) {
	for (int i = 0; i < nLen; ++i) {
		PutCharU8C(pUA[i]);
	}
}

void SavePNG::PutCharU16LC(unsigned short uVla) {
	PutCharU8C(uVla & 0xff);
	PutCharU8C((uVla >> 8) & 0xff);
}

void SavePNG::PutCharU32C(unsigned uVal) {
	PutCharU8C((uVal >> 24) & 0xff);
	PutCharU8C((uVal >> 16) & 0xff);
	PutCharU8C((uVal >> 8) & 0xff);
	PutCharU8C(uVal & 0xff);
}

void SavePNG::PutCharBeginBlock(char* pStr, unsigned nLen) {
	PutCharU32(nLen);
	m_nCurCRC = ~0U;
	PutCharU8AC(pStr, 4);
}

void SavePNG::PutCharEndBlock() {
	PutCharU32C(~m_nCurCRC);
}
