#include <math.h>
#include <memory.h>
#include <stdlib.h>
#include <time.h>
#include "SavePNG.h"

#define WIDTH 512
#define HEIGHT 512
#define PI 3.1415926f
#define TWO_PI 6.28318530718f
#define N 64
#define MAX_STEP 64
#define MAX_DISTANCE 2.0f
#define EPSILON 1e-6f

typedef struct { float sd, emissive; } Result;

unsigned char* g_pDrawBuff;

float Sample(float x, float y);
float Trace(float x, float y, float dx, float dy);
float CircleSDF(float x, float y, float cx, float cy, float r);
Result Scene(float x, float y);
Result UnionOp(Result a, Result b);
Result IntersectOp(Result a, Result b);
Result SubtractOp(Result a, Result b);

int main() {
	srand(time(NULL));
	SavePNG* pPng = SavePNG::CreatePNG("BaseLightCSG.png", WIDTH, HEIGHT, false);
	unsigned char* pPixcel = pPng->GetPNGBuff();
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x, pPixcel += 3) {
			pPixcel[0] = pPixcel[1] = pPixcel[2] = (int)(fminf(
				Sample((float)x / WIDTH, (float)y / HEIGHT) * 255.0f, 255.0f));
		}
	}
	pPng->Save();
	delete pPng;

	return 0;
}

float Sample(float x, float y) {
	float sum = 0.0f;
	for (int i = 0; i < N; ++i) {
		//float a = TWO_PI * i / N;
		float a = TWO_PI *(i + (float)rand() / RAND_MAX) / N;
		sum += Trace(x, y, cosf(a), sinf(a));
	}
	return sum / N;
}

float Trace(float x, float y, float dx, float dy) {
	float t = 0.001f;
	for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; ++i) {
		Result r = Scene(x + dx * t, y + dy * t);
		if (r.sd < EPSILON) {
			return r.emissive;
		}
		t += r.sd;
	}
	return 0.0f;
}

float CircleSDF(float x, float y, float cx, float cy, float r) {
	float ux = x - cx, uy = y - cy;
	return sqrtf(ux * ux + uy * uy) - r;
}

Result Scene(float x, float y) {
	//Result r1 = { CircleSDF(x, y, 0.3f, 0.3f, 0.1f), 2.0f };
	//Result r2 = { CircleSDF(x, y, 0.3f, 0.7f, 0.05f), 1.0f };
	//Result r3 = { CircleSDF(x, y, 0.7f, 0.5f, 0.1f), 0.0f };
	//return UnionOp(UnionOp(r1, r2), r3);
	Result r1 = { CircleSDF(x, y, 0.4f, 0.5f, 0.2f), 1.0f };
	Result r2 = { CircleSDF(x, y, 0.6f, 0.5f, 0.2f), 0.8f };
	//return UnionOp(r1, r2);
	//return IntersectOp(r1, r2);
	return SubtractOp(r1, r2);
}

Result UnionOp(Result a, Result b) {
	return a.sd < b.sd ? a : b;
}

Result IntersectOp(Result a, Result b) {
	Result r = a.sd < b.sd ? a : b;
	r.sd = a.sd > b.sd ? a.sd : b.sd;
	return r;
}

Result SubtractOp(Result a, Result b) {
	Result r = a;
	if (b.sd < 0) {
		r.sd = (a.sd > -b.sd) ? a.sd : -b.sd;
	}
	return r;
}
