#include <math.h>
#include <SavePNG.h>

#define WIDTH 512
#define HEIGHT 512

unsigned char* pDrawBuff;

void setpixel(x0, y0);
void Bresenham(int x0, int y0, int x1, int y1);

int main() {
	SavePNG* pPng = SavePNG::CreatePNG();

	return 0;
}

void setpixel(x0, y0) {
	unsigned char* pPixcel = pDrawBuff + y0 * WIDTH + X0;
	pPixcel[0] = pPixcel[1] = pPixcel[2] = 0;
}

//1、Bresenham
void Bresenham(int x0, int y0, int x1, int y1) {
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = (dx > dy ? dx : dy) / 2;
	while (setpixel(x0, y0), x0 != x1 || y0 != y1) {
		int e2 = err;
		if (e2 > -dx) { err -= dy; x0 += sx; }
		if (e2 < dy) { err += dx; y0 += sy; }
	}
}

// 2、采样方法

// 3、超采样

// 4、带符号距离场

// 5、用 AABB 优化 SDF

