#include "imgrenderer.h"

int main()
{
	renderer r;

	renderer_init(&r, 1600, 900);

	renderer_setColor(&r, COLORVALUE_BLACK);

	for (int y = 0; y < r.imgH-200; y += 100)
	{
		for (int x = 0; x < r.imgW-200; x += 100)
		{
			renderer_fillRect(&r, x, y, 50, 50);
		}
	}

	renderer_fillRect(&r, 1590, 850, 200, 200);

	renderer_generateImage(&r, "img.ppm");

	renderer_destroy(&r);

	return 0;
}
