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
#define MAX_STEP 10
#define MAX_DISTANCE 2.0f
#define EPSILON 1e-6f

unsigned char* g_pDrawBuff;

float Sample(float x, float y);
float Trace(float x, float y, float dx, float dy);
float CircleSDF(float x, float y, float cx, float cy, float r);

int main() {
	srand(time(NULL));
	SavePNG* pPng = SavePNG::CreatePNG("BaseLight.png", WIDTH, HEIGHT, false);
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
		float a = TWO_PI *(i + (float)rand()/RAND_MAX )/ N;
		sum += Trace(x, y, cosf(a), sinf(a));
	}
	return sum / N;
}

float Trace(float x, float y, float dx, float dy) {
	float t = 0.0f;
	for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; ++i) {
		float sd = CircleSDF(x + dx * t, y + dy * t, 0.5f, 0.5f, 0.1f);
		if (sd < EPSILON) {
			return 2.0f;
		}
		t += sd;
	}
	return 0.0f;
}

float CircleSDF(float x, float y, float cx, float cy, float r) {
	float ux = x - cx, uy = y - cy;
	return sqrt(ux * ux + uy * uy) - r;
}
