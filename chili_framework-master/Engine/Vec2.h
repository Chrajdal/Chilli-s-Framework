/******************************************************************************************
*	Chili DirectX Framework Version 16.10.01											  *
*	Vec2.h																				  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include <algorithm>
#include <math.h>

constexpr float PI = 3.14159265f;
constexpr double PI_D = 3.1415926535897932;

template <typename T>
inline auto sq(const T& x)
{
	return x * x;
}

template<typename T>
inline T wrap_angle(T theta)
{
	const T modded = fmod(theta, (T)2.0 * (T)PI_D);
	return (modded > (T)PI_D) ?
		(modded - (T)2.0 * (T)PI_D) :
		modded;
}

template<typename T>
inline T interpolate(const T& src, const T& dst, float alpha)
{
	return src + (dst - src) * alpha;
}

template <typename T>
class Cvector2
{
public:
	Cvector2(const T & x = {}, const T & y = {})
		:
		x(x),
		y(y)
	{}
	Cvector2(const Cvector2& vect)
		:
		Cvector2(vect.x, vect.y)
	{}
	template <typename T2>
	explicit operator Cvector2<T2>() const
	{
		return{ (T2)x,(T2)y };
	}
	T		LenSq() const
	{
		return sq(*this);
	}
	T		Len() const
	{
		return sqrt(LenSq());
	}
	Cvector2&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	Cvector2	GetNormalized() const
	{
		Cvector2 norm = *this;
		norm.Normalize();
		return norm;
	}
	Cvector2	operator-() const
	{
		return Cvector2(-x, -y);
	}
	Cvector2&	operator=(const Cvector2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	Cvector2&	operator+=(const Cvector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	Cvector2&	operator-=(const Cvector2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T		operator*(const Cvector2 &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	Cvector2	operator+(const Cvector2 &rhs) const
	{
		return Cvector2(*this) += rhs;
	}
	Cvector2	operator-(const Cvector2 &rhs) const
	{
		return Cvector2(*this) -= rhs;
	}
	Cvector2&	operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	Cvector2	operator*(const T &rhs) const
	{
		return Cvector2(*this) *= rhs;
	}
	Cvector2&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	Cvector2	operator/(const T &rhs) const
	{
		return Cvector2(*this) /= rhs;
	}
	bool	operator==(const Cvector2 &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=(const Cvector2 &rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
};

typedef Cvector2<float> float2;
typedef Cvector2<double> double2;
typedef Cvector2<int> int2;