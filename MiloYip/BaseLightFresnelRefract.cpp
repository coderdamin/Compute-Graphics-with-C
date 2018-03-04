#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "SavePNG.h"

#define WIDTH 512
#define HEIGHT 512
#define N 256
#define PI 3.1415926f
#define TWO_PI 6.28318530718f
#define MAX_STEP 64
#define MAX_DISTANCE 5.0f
#define EPSILON 1e-6f
#define BIAS 1e-4f
#define MAX_DEPTH 3

typedef struct {
	float sd, emissive, reflectivity, eta;
} Result;

float Sample(float fx, float fy);
float Trace(float fx, float fy, float fdx, float fdy, int ndepth = 0);
void Gradient(float fx, float fy, float* fnx, float* fny);
void Reflect(float fix, float fiy, float fnx, float fny, float* frx, float* fry);
int	Refract(float fix, float fiy, float fnx, float fny, float feta, float* frx, float* fry);
float Fresnel(float fcosi, float fcost, float fetai, float fetat);
float Schlick(float fcosi, float fcost, float fetai, float fetat);
Result Scene(float fx, float fy);

float CircleSDF(float x, float y, float cx, float cy, float r);
float BoxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy);
float PlaneSDF(float x, float y, float px, float py, float nx, float ny);

Result UnionOp(Result a, Result b);
Result IntersectOp(Result a, Result b);
Result SubtractOp(Result a, Result b);

int main() {
	srand(time(nullptr));
	SavePNG* pPng = SavePNG::CreatePNG("BaseLightFresnelRefract.png", WIDTH, HEIGHT, false);
	unsigned char* pPixcel = pPng->GetPNGBuff();
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x, pPixcel += 3) {
			pPixcel[0] = pPixcel[1] = pPixcel[2] = (int)(fminf(
				Sample((float)x / WIDTH, (float)y / HEIGHT) * 255.0f, 255.0f));
			//float fnx, fny;
			//Gradient((float)x / WIDTH, (float)y / HEIGHT, &fnx, &fny);
			//pPixcel[0] = (int)((fmaxf(fminf(fnx, 1.0f), -1.0f)*0.5f + 0.5f)*255.0f);
			//pPixcel[1] = (int)((fmaxf(fminf(fny, 1.0f), -1.0f)*0.5f + 0.5f)*255.0f);
			//pPixcel[2] = 0;
		}
	}
	pPng->Save();
	delete pPng;

	return 0;
}

float Sample(float fx, float fy) {
	float fSum = 0.0f;
	for (int i = 0; i < N; ++i) {
		float a = TWO_PI * (i + (float)rand() / RAND_MAX) / N;
		fSum += Trace(fx, fy, cosf(a), sinf(a));
	}
	return fSum / N;
}

float Trace(float fx, float fy, float fdx, float fdy, int ndepth) {
	float t = 0.001f;
	float sign = Scene(fx, fy).sd > 0.0f ? 1.0f : -1.0f;
	for (int i = 0; i < MAX_STEP && t < MAX_DISTANCE; ++i) {
		float x = fx + fdx * t, y = fy + fdy * t;
		Result r = Scene(x, y);
		if (r.sd * sign < EPSILON) {
			float fSum = r.emissive;
			if (ndepth < MAX_DEPTH && (r.reflectivity > 0.0f || r.eta > 0.0f)) {
				float nx, ny, rx, ry, refl = r.reflectivity;
				Gradient(x, y, &nx, &ny);
				nx *= sign; ny *= sign;
				if (r.eta > 0.0f) {
					if (Refract(fdx, fdy, nx, ny, sign<0.0f ? r.eta:1.0f/ r.eta, &rx, &ry) != 0) {
						float cosi = -(fdx*nx + fdy * ny);
						float cost = -(rx*nx + ry * ny);
						refl = sign < 0.0f ? Fresnel(cosi, cost, r.eta, 1.0f) : Fresnel(cosi, cost, 1.0f, r.eta);
						fSum += (1.0f - refl) * Trace(x - nx * BIAS, y - ny * BIAS, rx, ry, ndepth + 1);
					}
					else {
						refl = 1.0f;
					}
				}
				if (refl > 0.0f) {
					Reflect(fdx, fdy, nx, ny, &rx, &ry);
					fSum += refl * Trace(x + nx * BIAS, y + ny * BIAS, rx, ry, ndepth + 1);
				}
			}
			return fSum;
		}
		t += r.sd * sign;
	}
	return 0.0f;
}

void Gradient(float fx, float fy, float* fnx, float* fny) {
	*fnx = (Scene(fx + EPSILON, fy).sd - Scene(fx - EPSILON, fy).sd)*(0.5f / EPSILON);
	*fny = (Scene(fx, fy + EPSILON).sd - Scene(fx, fy - EPSILON).sd)*(0.5f / EPSILON);
}

void Reflect(float fix, float fiy, float fnx, float fny, float* frx, float* fry) {
	float idotn2 = (fix * fnx + fiy * fny) * 2.0f;
	*frx = fix - idotn2 * fnx;
	*fry = fiy - idotn2 * fny;
}

// t = (n1/n2)¡¤i-((n1/n2)(i¡¤n)+(1-(n1/n2)^2(1-(i¡¤n)^2))^(-2))¡¤n
int	Refract(float fix, float fiy, float fnx, float fny, float feta, float* frx, float* fry) {
	float idotn = fix * fnx + fiy * fny;
	float k = 1.0f - feta * feta*(1.0f - idotn * idotn);
	if (k < 0.0f) {
		return 0;
	}
	float a = feta * idotn + sqrtf(k);
	*frx = feta * fix - a * fnx;
	*fry = feta * fiy - a * fny;
	return 1;
}

float Fresnel(float fcosi, float fcost, float fetai, float fetat) {
	float rs = (fetat*fcosi - fetai * fcost) / (fetat*fcosi + fetai * fcost);
	float rp = (fetai*fcosi - fetat * fcost) / (fetai*fcosi + fetat * fcost);
	return (rs * rs + rp * rp)*0.5f;
}

// µ¼Ìå·ÆÄá¶û½üËÆ R(0) + (1-R(0))(1-cosi)^5
float Schlick(float fcosi, float fcost, float fetai, float fetat) {
	float r0 = (fetai - fetat) / (fetai + fetat);
	r0 *= r0;
	float a = 1.0f - (fetai < fetat ? fcosi : fcost);
	float aa = a * a;
	return r0 + (1.0f - r0) * aa * aa * a;
}

float CircleSDF(float x, float y, float cx, float cy, float r) {
	float ux = x - cx, uy = y - cy;
	return sqrtf(ux * ux + uy * uy) - r;
}

float BoxSDF(float x, float y, float cx, float cy, float theta, float sx, float sy) {
	float costheta = cosf(theta), sintheta = sinf(theta);
	float dx = fabs((x - cx)*costheta + (y - cy)*sintheta) - sx;
	float dy = fabs((y - cy)*costheta - (x - cx)*sintheta) - sy;
	float ax = fmaxf(dx, 0.0f), ay = fmaxf(dy, 0.0f);
	return fminf(fmaxf(dx, dy), 0.0f) + sqrtf(ax*ax + ay * ay);
}

float PlaneSDF(float x, float y, float px, float py, float nx, float ny) {
	return (x - px)*nx + (y - py)*ny;
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

Result Scene(float fx, float fy) {
	//Result r1 = { CircleSDF(fx, fy, 0.4f, 0.2f, 0.1f), 2.0f, 0.0f, 0.0f};
	//Result r2 = { BoxSDF(fx, fy, 0.5f, 0.8f, TWO_PI / 16.0f, 0.1f, 0.1f), 0.0f, 0.9f, 1.5f};
	//Result r3 = { BoxSDF(fx, fy, 0.8f, 0.5f, TWO_PI / 16.0f, 0.1f, 0.1f), 0.0f, 0.9f, 1.5f};
	//return UnionOp(UnionOp(r1, r2), r3);

	//Result a = { CircleSDF(fx, fy, -0.2f, -0.2f, 0.1f), 10.0f, 0.0f, 0.0f };
	//Result b = { BoxSDF(fx, fy, 0.5f, 0.5f, 0.0f, 0.3, 0.2f), 0.0f, 0.2f, 1.5f };
	//return UnionOp(a, b);

	Result c = { CircleSDF(fx, fy, 0.5f, -0.5f, 0.05f), 20.0f, 0.0f, 0.0f };
	Result d = { CircleSDF(fx, fy, 0.5f, 0.2f, 0.35f), 0.0f, 0.2f, 1.5f };
	Result e = { CircleSDF(fx, fy, 0.5f, 0.8f, 0.35f), 0.0f, 0.2f, 1.5f };
	return UnionOp(c, IntersectOp(d, e));
}
