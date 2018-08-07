#pragma once

#include "Bitmap.h"

class Surface
{
public:
	Surface(const Bitmap * src, int fromx, int fromy, int width, int height);
	Surface(const Surface & src);
	Surface & operator = (const Surface & src);
	~Surface(void);

	int from_x(void) const;
	int from_y(void) const;
	int width(void) const;
	int height(void) const;
	Color * get_data(void) const;

private:
	int x, y, w, h;
	Color * m_data;
};