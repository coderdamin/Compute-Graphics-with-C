#include "Scene.h"
#include "SavePNG.h"
#include "Example.h"
#include <math.h>


#define WIDTH 512
#define HEIGHT 512


int main() {
	SavePNG* pPng = SavePNG::CreatePNG("Reflection_3.png", WIDTH, HEIGHT, false);
	Scene* pScene = GenerateScene2();

	unsigned char* pcPixelBuff = pPng->GetPNGBuff();
	for (int y = 0; y < HEIGHT; ++y) {
		for (int x = 0; x < WIDTH; ++x, pcPixelBuff += 3) {
			Color color = pScene->Sample(Vector((float)x / WIDTH, (float)y / HEIGHT));
			pcPixelBuff[0] = (int)fminf(color.m_fr * 255.0f, 255.0f);
			pcPixelBuff[1] = (int)fminf(color.m_fg * 255.0f, 255.0f);
			pcPixelBuff[2] = (int)fminf(color.m_fb * 255.0f, 255.0f);
		}
	}
	if (pScene != nullptr) {
		delete pScene;
	}
	if (pPng != nullptr) {
		pPng->Save();
		delete pPng;
	}
	return 0;
}
