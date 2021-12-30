#ifndef _IMGRENDERER_INCLUDE_IMGRENDERER_H_
#define _IMGRENDERER_INCLUDE_IMGRENDERER_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Please understand that this is not going to be an efficent method of generating an image file.
 * The image file generated is a .ppm format which is very decompressed and takes up significantly
 * more space than a .png for example.
 * 
 */

#define COLORVALUE_WHITE 0xFFFFFF
#define COLORVALUE_RED 0xFF0000
#define COLORVALUE_ORANGE 0xFF8921
#define COLORVALUE_BROWN 0x4D2802
#define COLORVALUE_YELLOW 0xFFFF00
#define COLORVALUE_GREEN 0x1C8A1A
#define COLORVALUE_CYAN 0x0FB5B8
#define COLORVALUE_BLUE 0x1316EB
#define COLORVALUE_PURPLE 0x5D0DBF
#define COLORVALUE_MAGENTA 0xFF00FF
#define COLORVALUE_BLACK 0x000000

typedef struct renderer
{
	int offsetX, offsetY;

	int imgW, imgH;

	uint8_t red;
	uint8_t green;
	uint8_t blue;

	uint8_t* buffer;
} renderer;

void createPPM(const char* path, const int width, const int height, const unsigned char* data);
uint64_t getIndex(const uint64_t x, const uint64_t y, const uint64_t imgW);
void renderer_init(renderer* r, int imgW, int imgH);
void renderer_destroy(renderer* r);
bool isBoundPoint(renderer* r, int x, int y);//tests if point given is within image boundary
void renderer_setColor(renderer* r, uint32_t color);//most significant byte(left-most) is unused
void renderer_fillScreen(renderer* r);
void renderer_drawPixel(renderer* r, int x, int y);
void renderer_fillRect(renderer* r, int x, int y, int w, int h);
void renderer_fillOval(renderer* r, int x, int y, int w, int h);
void renderer_drawLine(renderer* r, int x0, int y0, int x1, int y1);
void renderer_drawTriangle(renderer* r, int x0, int y0, int x1, int y1, int x2, int y2);

/**
 * Remember to include the .ppm file extension!
 */
void renderer_generateImage(renderer* r, const char* path);

#endif
