#pragma once

#include <algorithm>
#include <vector>
#include <chrono>
#include "Graphics.h"
#include "Colors.h"

using namespace std;
using namespace std::chrono;

//-----------------------------------------------------------------------------
template<typename T>
struct Tpoint
{
	Tpoint(const T & _x = 0, const T & _y = 0)
		: m_x(_x), m_y(_y)
	{}
	Tpoint(const Tpoint<T> & src)
		: m_x(src.m_x), m_y(src.m_y)
	{}
	Tpoint(const Tpoint<T> && src)
		: m_x(move(src.m_x)), m_y(move(src.m_y))
	{}
	Tpoint<T> & operator = (const Tpoint<T> & src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}
	Tpoint<T> & operator = (const Tpoint<T> && src)
	{
		if (this == &src)
			return *this;
		m_x = move(src.m_x);
		m_y = move(src.m_y);
		return *this;
	}
	inline bool operator == (const Tpoint<T> & rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	inline bool operator != (const Tpoint<T> & rhs) const
	{
		return !(*this == rhs);
	}
	T m_x;
	T m_y;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template<typename T>
struct Trect
{
	constexpr Trect(const T & x1 = {}, const T & y1 = {}, const T & x2 = {}, const T & y2 = {})
		: m_upleft(x1,y1), m_downright(x2, y2)
	{}
	constexpr Trect(const Tpoint<T> & upleft = {}, const Tpoint<T> & downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}
	constexpr Trect(const Trect<T> & src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}
	constexpr Trect(const Trect<T> && src)
		: m_upleft(move(src.m_upleft)), m_downright(move(src.m_downright))
	{}
	constexpr Trect<T> & operator = (const Trect<T> & src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}
	constexpr Trect<T> & operator = (const Trect<T> && src)
	{
		if (this == &src)
			return *this;
		m_upleft = move(src.m_upleft);
		m_downright = move(src.m_downright);
		return *this;
	}
	Tpoint<T> m_upleft;
	Tpoint<T> m_downright;

	void draw(Graphics & gfx, const Color & c) const
	{
		gfx.draw_line_s(m_upleft.m_x, m_upleft.m_y, m_downright.m_x, m_upleft.m_y, c);
		gfx.draw_line_s(m_upleft.m_x, m_upleft.m_y, m_upleft.m_x, m_downright.m_y, c);
		gfx.draw_line_s(m_upleft.m_x, m_downright.m_y, m_downright.m_x, m_downright.m_y, c);
		gfx.draw_line_s(m_downright.m_x, m_upleft.m_y, m_downright.m_x, m_downright.m_y, c);
	}
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CTimer
{
public:
	CTimer(void)
	{
		start = chrono::system_clock::now();
	}
	inline double elapsed(void) const
	{
		return (double)(chrono::duration_cast<chrono::milliseconds>
			(chrono::system_clock::now() - start)).count();
	}
	inline void restart(void)
	{
		start = chrono::system_clock::now();
	}
private:
	chrono::time_point<chrono::system_clock> start;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
int random_int(int min, int max);
float random_between(const float & min, const float & max);
double random_between(const double & min, const double & max);
long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b);
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b);
double sq_distance(const Tpoint<float> & a, const Tpoint<float> & b);
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b);
void gen_random(string & str, const int len);
void gen_randascii(string & str, const int len);
//-----------------------------------------------------------------------------
