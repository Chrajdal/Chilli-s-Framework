#include "test.h"	


#ifndef _UTILS_ASDFASDFQWEQWEAS1DF1Q56WE1F65QWE1F58Q4WE89F4QW8EF_
#define _UTILS_ASDFASDFQWEQWEAS1DF1Q56WE1F65QWE1F58Q4WE89F4QW8EF_

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

template<typename T>
Tpoint<T> get_closest_p(const Trect<T> & rect, const Tpoint<T> & p)
{
	/* 1 | 2 | 3
	---+---+---
	4 | 5 | 6
	---+---+---
	7 | 8 | 9 */

	if (p.m_x < rect.m_upleft.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 1
			return Tpoint<T>(rect.m_upleft);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 7
			return Tpoint<T>(rect.m_upleft.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 4
			return Tpoint<T>(rect.m_upleft.m_x, p.m_y);
		}
	}
	else if (p.m_x > rect.m_downright.m_x)
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 3
			return Tpoint<T>(rect.m_downright.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 9
			return Tpoint<T>(rect.m_downright);
		}
		else
		{	// no.: 6
			return Tpoint<T>(rect.m_downright.m_x, p.m_y);
		}
	}
	else
	{
		if (p.m_y < rect.m_upleft.m_y)
		{	// no.: 2
			return Tpoint<T>(p.m_x, rect.m_upleft.m_y);
		}
		else if (p.m_y > rect.m_downright.m_y)
		{	// no.: 8
			return Tpoint<T>(p.m_x, rect.m_downright.m_y);
		}
		else
		{	// no.: 5
			return p;
		}
	}
}
//-----------------------------------------------------------------------------

#endif // !_UTILS_ASDFASDFQWEQWEAS1DF1Q56WE1F65QWE1F58Q4WE89F4QW8EF_