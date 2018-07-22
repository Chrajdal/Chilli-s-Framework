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
struct Cvector2
{
	Cvector2(const T & _x, const T & _y)
		: m_x(_x), m_y(_y)
	{}
	Cvector2(const Cvector2<T> & src)
		: m_x(src.m_x), m_y(src.m_y)
	{}
	Cvector2(const Cvector2<T> && src)
		: m_x(move(src.m_x)), m_y(move(src.m_y))
	{}
	Cvector2<T> & operator = (const Cvector2<T> & src)
	{
		if (this == &src)
			return *this;
		m_x = src.m_x;
		m_y = src.m_y;
		return *this;
	}
	Cvector2<T> & operator = (const Cvector2<T> && src)
	{
		if (this == &src)
			return *this;
		m_x = move(src.m_x);
		m_y = move(src.m_y);
		return *this;
	}
	inline bool operator == (const Cvector2<T> & rhs) const
	{
		return m_x == rhs.m_x && m_y == rhs.m_y;
	}
	inline bool operator != (const Cvector2<T> & rhs) const
	{
		return !(*this == rhs);
	}

	inline Cvector2<double> operator - (const Cvector2<T> & rhs) const
	{
		return std::move(Cvector2<double>(m_x - rhs.m_x, m_y - rhs.m_y));
	}
	inline Cvector2<double> & operator -= (const Cvector2<T> & rhs)
	{
		m_x -= rhs.m_x;
		m_y -= rhs.m_y;
		return *this;
	}

	inline Cvector2<double> operator + (const Cvector2<T> & rhs) const
	{
		return std::move(Cvector2<double>(m_x + rhs.m_x, m_y + rhs.m_y));
	}
	inline Cvector2<double> & operator += (const Cvector2<T> & rhs)
	{
		m_x += rhs.m_x;
		m_y += rhs.m_y;
		return *this;
	}

	inline Cvector2<double> operator * (const Cvector2<T> & rhs) const
	{
		return std::move(Cvector2<double>(m_x * rhs.m_x, m_y * rhs.m_y));
	}
	inline Cvector2<double> & operator *= (const Cvector2<T> & rhs)
	{
		m_x *= rhs.m_x;
		m_y *= rhs.m_y;
		return *this;
	}
	inline Cvector2<double> & operator * (const T & rhs) const
	{
		return Cvector2<double>(m_x * rhs, m_y * rhs);
	}
	inline Cvector2<double> & operator *= (const T & rhs)
	{
		m_x *= rhs;
		m_y *= rhs;
		return *this;
	}

	inline Cvector2<double> operator / (const Cvector2<T> & rhs) const
	{
		return std::move(Cvector2<double>(m_x / rhs.m_x, m_y / rhs.m_y));
	}
	inline Cvector2<double> & operator /= (const Cvector2<T> & rhs)
	{
		m_x /= rhs.m_x;
		m_y /= rhs.m_y;
		return *this;
	}
	inline Cvector2<double> & operator / (const T & rhs)
	{
		return Cvector2<double>(m_x / rhs, m_y / rhs);
	}
	inline Cvector2<double> & operator /= (const T & rhs) const
	{
		m_x /= rhs;
		m_y /= rhs;
		return *this;
	}

	inline T magnitude(void) const
	{
		return sqrt((m_x*m_x) + (m_y*m_y));
	}

	inline Cvector2<T> & normalize(void)
	{
		return *this / magnitude();
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
	constexpr Trect(const Cvector2<T> & upleft = {}, const Cvector2<T> & downright = {})
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
	Cvector2<T> m_upleft;
	Cvector2<T> m_downright;

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
long sq_distance(const Cvector2<int> & a, const Cvector2<int> & b);
long long sq_distance(const Cvector2 <long> & a, const Cvector2<long> & b);
double sq_distance(const Cvector2<float> & a, const Cvector2<float> & b);
double sq_distance(const Cvector2<double> & a, const Cvector2<double> & b);
void gen_random(string & str, const int len);
void gen_randascii(string & str, const int len);
//-----------------------------------------------------------------------------
