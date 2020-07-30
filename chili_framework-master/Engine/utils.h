#pragma once

#include <algorithm>
#include <vector>
#include <chrono>
#include <string>
#include <numeric>
#include <cmath>

#include "Vec2.h"
#include "Vec3.h"
#include "Mat2.h"
#include "Mat3.h"

//using namespace std;
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
	Trect(const Tpoint<T> & upleft = {}, const Tpoint<T> & downright = {})
		: m_upleft(upleft), m_downright(downright)
	{}
	Trect(const Trect<T> & src)
		: m_upleft(src.m_upleft), m_downright(src.m_downright)
	{}
	Trect(const Trect<T> && src)
		: m_upleft(move(src.m_upleft)), m_downright(move(src.m_downright))
	{}
	Trect<T> & operator = (const Trect<T> & src)
	{
		if (this == &src)
			return *this;
		m_upleft = src.m_upleft;
		m_downright = src.m_downright;
		return *this;
	}
	Trect<T> & operator = (const Trect<T> && src)
	{
		if (this == &src)
			return *this;
		m_upleft = move(src.m_upleft);
		m_downright = move(src.m_downright);
		return *this;
	}
	Tpoint<T> m_upleft;
	Tpoint<T> m_downright;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class CTimer
{
public:
	CTimer(void)
	{
		start = std::chrono::system_clock::now();
	}
	inline double elapsed(void) const
	{
		return (double)(std::chrono::duration_cast<std::chrono::milliseconds>
			(std::chrono::system_clock::now() - start)).count();
	}
	inline void restart(void)
	{
		start = std::chrono::system_clock::now();
	}
private:
	std::chrono::time_point<std::chrono::system_clock> start;
};
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
template <typename T, typename U, typename V, typename W, typename X>
T map_value(const T& value, const U& inmin, const V& inmax, const W& outmin, const X& outmax)
{
	return outmin + (value - inmin) * (outmax - outmin) / (inmax - inmin);
}

//float2 rotate_point(float cx, float cy, float angle, float2 p) {
//
//	return float2(std::cos(angle) * (p.x - cx) - std::sin(angle) * (p.y - cy) + cx,
//		std::sin(angle) * (p.x - cx) + std::cos(angle) * (p.y - cy) + cy);
//}

int random_int(int min, int max);
float random_between(const float & min, const float & max);
double random_between(const double & min, const double & max);
long sq_distance(const Tpoint<int> & a, const Tpoint<int> & b);
long long sq_distance(const Tpoint <long> & a, const Tpoint<long> & b);
double sq_distance(const Tpoint<float> & a, const Tpoint<float> & b);
double sq_distance(const Tpoint<double> & a, const Tpoint<double> & b);
void gen_random(std::string & str, const int len);
void gen_randascii(std::string & str, const int len);
//-----------------------------------------------------------------------------
