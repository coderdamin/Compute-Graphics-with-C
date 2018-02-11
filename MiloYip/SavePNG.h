#include <stdio.h>

class SavePNG {
private:
	SavePNG();

public:
	static SavePNG* CreatePNG(char* fileName, unsigned width, unsigned height, bool alpha);
	~SavePNG();

public:
	//void DrawPNG();
	unsigned char* GetPNGBuff() { return m_pPNGBuff; }
	void Save();

private:
	void PutChar(char val);
	void PutCharU8A(char* pUA, unsigned nLen);
	void PutCharU32(unsigned uVal);
	void PutCharU8C(unsigned char uVla);
	void PutCharU8AC(char* pUA, unsigned nLen);
	void PutCharU16LC(unsigned short uVla);
	void PutCharU32C(unsigned uVal);
//#define SVPNG_U8ADLER(u) do { SVPNG_U8C(u); a = (a + (u)) % 65521; b = (b + a) % 65521; } while(0)
	void PutCharBeginBlock(char* pStr, unsigned nLen);
	void PutCharEndBlock();

private:
	bool m_bAlpha;
	FILE* m_fp;
	unsigned char* m_pPNGBuff;
	unsigned int m_nWidth;
	unsigned int m_nHeight;
	unsigned int m_nCurCRC;
};