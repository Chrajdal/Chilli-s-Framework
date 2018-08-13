#include "Surface.h"

Surface::Surface(const Bitmap * src, int fromx, int fromy, int width, int height)
{
	x = fromx;
	y = fromy;
	w = width;
	h = height;
	m_data = new Color[w*h];
	//std::fill_n(m_data, w * h, src->data()[x + y * w]);
	
	int idx = 0;
	for (int i = fromy; i < fromy + height; ++i)
	{
		std::copy_n(src->data() + i * src->width + fromx, width, m_data + (idx++) * width);
	}
}

Surface::Surface(const Surface & src)
	:
	x(src.x),
	y(src.y),
	w(src.w),
	h(src.h)
{
	m_data = new Color[w*h];
	std::copy_n(src.m_data, w * h, m_data);
}

Surface & Surface::operator = (const Surface & src)
{
	if (this == &src)
		return *this;

	x = src.x;
	y = src.y;
	w = src.w;
	h = src.h;

	delete m_data;
	m_data = new Color[w*h];
	std::copy_n(src.m_data, w*h, m_data);

	return *this;
}

Surface::~Surface(void)
{
	delete m_data;
}

int Surface::from_x(void) const
{
	return x;
}

int Surface::from_y(void) const
{
	return y;
}

int Surface::width(void) const
{
	return w;
}

int Surface::height(void) const
{
	return h;
}

Color * Surface::get_data(void) const
{
	return m_data;
}