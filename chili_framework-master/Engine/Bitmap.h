#pragma once

#include "Colors.h"
#include "Graphics.h"
#include <fstream>
#include <cassert>

class Bitmap
{
public:
	Bitmap(const string & filename);
	Bitmap(const Bitmap & rhs);
	~Bitmap();
	Bitmap& operator = (const Bitmap & rhs);
	void PutPixel(int x, int y, const Color & c);
	Color GetPixel(int x, int y) const;
	void Draw(Graphics & gfx, int x, int y, const Color & alpha) const;
	void DrawPart(Graphics & gfx, int x, int y,int fromx, int fromy, int tox, int toy, const Color & alpha) const;
	void resize(int newwidth, int newheight);
	void leave_only_this_color(const Color & color);


	inline int get_width(void) const { return width; }
	inline int get_height(void) const { return height; }
private:
	Color* pPixels = nullptr;
	int width;
	int height;

	typedef struct tagBITMAPINFOHEADER
	{
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