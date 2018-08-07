#pragma once

#include "Colors.h"
#include <fstream>
#include <cassert>

#include <d3d11.h>
#include <wrl.h>

class Bitmap
{
public:
	Bitmap(const string & filename);
	Bitmap(const Bitmap & rhs);
	~Bitmap();
	Bitmap& operator = (const Bitmap & rhs);
	void PutPixel(int x, int y, const Color & c);
	Color GetPixel(int x, int y) const;

	Color * data(void) const;

	//void resize(int newwidth, int newheight);
private:
	Color * pPixels = nullptr;
public:
	int width;
	int height;
private:
	typedef struct tagBITMAPINFOHEADER {
		DWORD biSize;
		LONG  biWidth;
		LONG  biHeight;
		WORD  biPlanes;
		WORD  biBitCount;
		DWORD biCompression;
		DWORD biSizeImage;
		LONG  biXPelsPerMeter;
		LONG  biYPelsPerMeter;
		DWORD biClrUsed;
		DWORD biClrImportant;
	} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
};