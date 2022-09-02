#include "imgrenderer.h"

void renderer_init(renderer* r, int imgW, int imgH)
{
	r->offsetX = 0;
	r->offsetY = 0;

	r->imgW = imgW;
	r->imgH = imgH;

	r->red = 0x00;
	r->green = 0x00;
	r->blue = 0x00;

	uint64_t size = sizeof(unsigned char)*imgW*imgH*3;

	r->buffer = (uint8_t*)malloc(size);

	for (uint64_t i = 0; i < size; i++)
	{
		r->buffer[i] = 0xFF;
	}
}

void renderer_destroy(renderer* r)
{
	free(r->buffer);
}
bool isBoundPoint(renderer* r, int x, int y)
{
	if ((x >= r->imgW) || (x < 0) || (y >= r->imgH) || (y < 0))
		return false;
	else
		return true;
}

uint64_t getIndex(const uint64_t x, const uint64_t y, const uint64_t imgW)
{
	return 3*(imgW*y + x);
}

double numberMap(double n, double start1, double stop1, double start2, double stop2)
{
	return (n - start1) / (stop1 - start1) * (stop2 - start2) + start2;
}

void renderer_setColor(renderer* r, uint32_t color)
{
	const uint32_t mask = 0xFF;

	color = color & 0x00FFFFFF;

	r->red = (color >> 8*2) & mask;
	r->green = (color >> 8*1) & mask;
	r->blue = color & mask;
}

void renderer_setIntColor(renderer* r, uint8_t red, uint8_t green, uint8_t blue)
{
	r->red = red;
	r->green = green;
	r->blue = blue;
}

void renderer_fillScreen(renderer* r)
{
	const int size = r->imgW*r->imgH;

	for (uint64_t i = 0; i < size; i += 3)
	{
		r->buffer[i + 0] = r->red;
		r->buffer[i + 1] = r->blue;
		r->buffer[i + 2] = r->green;
	}
}

void renderer_drawPixel(renderer* r, int x, int y)
{
	x += r->offsetX;
	y += r->offsetY;

	if (!isBoundPoint(r, x, y))
		return;
	
	uint64_t index = getIndex(x, y, r->imgW);

	r->buffer[index + 0] = r->red;
	r->buffer[index + 1] = r->green;
	r->buffer[index + 2] = r->blue;
}

void renderer_fillRect(renderer* r, int x, int y, int w, int h)
{
	if (x >= r->imgW || y >= r->imgH || w < 1 || h < 1)
	{
		return;
	}

	int left = 0 > x ? 0 : x;
	int top = 0 > y ? 0 : y;

	int visibleX = r->imgW-left-1;
	int visibleY = r->imgH-top-1;

	int clippedW = w < visibleX ? w : visibleX;
	int clippedH = h < visibleY ? h : visibleY;

	int right = left + clippedW;
	int bottom = top + clippedH;

	for (int j = top; j <= bottom; j++)
	{
		for (int i = left; i <= right; i++)
		{
			uint64_t index = getIndex(i, j, r->imgW);

			r->buffer[index + 0] = r->red;
			r->buffer[index + 1] = r->green;
			r->buffer[index + 2] = r->blue;
		}
	}
}

bool inCircle(double x, double y)
{
	if (x < 0 || x > 1 || y < 0 || y > 1)
		return false;
	else if (sqrt(((x - 0.5) * (x - 0.5)) + ((y - 0.5) * (y - 0.5))) > 0.5)
		return false;
	else
		return true;
}

/**
 * This function is going to map a circle in order to project into an oval
 */

void renderer_fillOval(renderer* r, const int x, const int y, const int w, const int h)
{
	if (x >= r->imgW || y >= r->imgH || w < 1 || h < 1)
	{
		return;
	}

	int left = 0 > x ? 0 : x;
	int top = 0 > y ? 0 : y;

	int visibleX = r->imgW-left-1;
	int visibleY = r->imgH-top-1;

	int clippedW = w < visibleX ? w : visibleX;
	int clippedH = h < visibleY ? h : visibleY;

	int right = left + clippedW;
	int bottom = top + clippedH;

	for (int j = top; j <= bottom; j++)
	{
		for (int i = left; i <= right; i++)
		{
			if (inCircle(
				numberMap(i, (double)x, (double)x + (double)w, 0, 1),
				numberMap(j, (double)y, (double)y + (double)h, 0, 1)
			)) {
				uint64_t index = getIndex(i, j, r->imgW);

				r->buffer[index + 0] = r->red;
				r->buffer[index + 1] = r->green;
				r->buffer[index + 2] = r->blue;
			}
		}
	}
}


void renderer_drawLine(renderer* r, int x0, int y0, int x1, int y1);
void renderer_drawTriangle(renderer* r, int x0, int y0, int x1, int y1, int x2, int y2);

void createPPM(const char* path, const int width, const int height, const unsigned char* data)
{
	FILE* f;

	const char lineBreak = 0x0a;
	const char space = ' ';
	const char signature[2] = {'P','6'};
	const char colorDepth[] = "255";
	const int colorDepth_l = 3;
	char width_str[16];
	char height_str[16];
	sprintf(width_str, "%d", width);
	sprintf(height_str, "%d", height);

	uint64_t data_length = (uint64_t)width*(uint64_t)height*(uint64_t)3;

	f = fopen(path, "wb");

	fwrite(signature, sizeof(char), 2, f);
	fwrite(&lineBreak, sizeof(char), 1, f);
	
	int length = strlen(width_str);

	fwrite(width_str, length, 1, f);

	fwrite(&space, sizeof(char), 1, f);

	length = strlen(height_str);

	fwrite(height_str, length, 1, f);

	fwrite(&lineBreak, sizeof(char), 1, f);

	fwrite(&colorDepth, colorDepth_l, 1, f);

	fwrite(&lineBreak, sizeof(char), 1, f);

	uint64_t counter = 0;

	for (uint64_t i = 0; i < data_length; i++)
	{
		uint8_t byte = data[i];

		fwrite(&byte, sizeof(unsigned char), 1, f);

		counter++;
	}

	fclose(f);
}

void renderer_generateImage(renderer* r, const char* path)
{
	createPPM(path, r->imgW, r->imgH, r->buffer);
}
