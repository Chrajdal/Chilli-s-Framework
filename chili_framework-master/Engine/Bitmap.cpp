#include "Bitmap.h"


Bitmap::Bitmap(const string& filename)
{
	ifstream file(filename, std::ios::binary);
	if (!file.is_open())
		throw filename + " cannot be opened.";

	BITMAPFILEHEADER bmFileHeader;
	file.read(reinterpret_cast<char*>(&bmFileHeader), sizeof(bmFileHeader));

	BITMAPINFOHEADER bmInfoHeader;
	file.read(reinterpret_cast<char*>(&bmInfoHeader), sizeof(bmInfoHeader));

	assert(bmInfoHeader.biBitCount == 24 || bmInfoHeader.biBitCount == 32);
	assert(bmInfoHeader.biCompression == BI_RGB);

	const bool is32b = bmInfoHeader.biBitCount == 32;

	width = bmInfoHeader.biWidth;

	// test for reverse row order and control
	// y loop accordingly
	int yStart;
	int yEnd;
	int dy;
	if (bmInfoHeader.biHeight < 0)
	{
		height = -bmInfoHeader.biHeight;
		yStart = 0;
		yEnd = height;
		dy = 1;
	}
	else
	{
		height = bmInfoHeader.biHeight;
		yStart = height - 1;
		yEnd = -1;
		dy = -1;
	}

	pPixels = new Color[width*height];

	file.seekg(bmFileHeader.bfOffBits);
	// padding is for the case of of 24 bit depth only
	const int padding = (4 - (width * 3) % 4) % 4;

	for (int y = yStart; y != yEnd; y += dy)
	{
		for (int x = 0; x < width; x++)
		{
			PutPixel(x, y, Color(file.get(), file.get(), file.get()));
			if (is32b)
			{
				file.seekg(1, std::ios::cur);
			}
		}
		if (!is32b)
		{
			file.seekg(padding, std::ios::cur);
		}
	}
}

Bitmap::Bitmap(const Bitmap& rhs)
	: width(rhs.width), height(rhs.height)
{
	pPixels = new Color[width * height];
	//const int nPixels = width * height;
	//for (int i = 0; i < nPixels; i++)
	//	pPixels[i] = rhs.pPixels[i];
	memcpy(pPixels, rhs.pPixels, sizeof(Color) * width * height);
}

Bitmap::~Bitmap()
{
	delete[] pPixels;
	pPixels = nullptr;
}

Bitmap& Bitmap::operator=(const Bitmap & rhs)
{
	width = rhs.width;
	height = rhs.height;

	delete[] pPixels;
	pPixels = new Color[width*height];

	//memcpy(pPixels, rhs.pPixels, sizeof(Color) * width * height);
	std::copy_n(rhs.pPixels, width * height, pPixels);
	return *this;
}

void Bitmap::PutPixel(int x, int y, const Color & c)
{
	pPixels[y * width + x] = c;
}

Color Bitmap::GetPixel(int x, int y) const
{
	return pPixels[y * width + x];
}

Color * Bitmap::data(void) const
{
	return pPixels;
}
