#include "utils.h"

template<typename T>
void draw_rect(Graphics & gfx, const Trect<T> & rect, const Color & c)
{
	for (int i = rect.m_upleft.m_x; i <= rect.m_downright.m_x; ++i)
	{
		if (i >= 0 && i < Graphics::ScreenWidth && rect.m_upleft.m_y >= 0 && rect.m_downright.m_y < Graphics::ScreenHeight)
		{
			gfx.PutPixel((int)i, (int)rect.m_upleft.m_y, c);
			gfx.PutPixel((int)i, (int)rect.m_downright.m_y, c);
		}
	}
	for (int i = rect.m_upleft.m_y; i <= rect.m_downright.m_y; ++i)
	{
		if (i >= 0 && i < Graphics::ScreenWidth && rect.m_upleft.m_y >= 0 && rect.m_downright.m_y < Graphics::ScreenHeight)
		{
			gfx.PutPixel((int)rect.m_upleft.m_x, (int)i, c);
			gfx.PutPixel((int)rect.m_downright.m_x, (int)i, c);
		}
	}
}
//-----------------------------------------------------------------------------
int random_int(int min, int max)
{
	int range = max - min + 1;
	return (rand() % range) + min;
}
float random_between(const float & min, const float & max)
{
	return (max - min) * ((float)rand() / (float)RAND_MAX) + min;
}
double random_between(const double & min, const double & max)
{
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
}

long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<float> & a, const Tpoint<float> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
//-----------------------------------------------------------------------------
