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
class _Vec2
{
public:
	_Vec2(const T & x = {}, const T & y = {})
		:
		x(x),
		y(y)
	{}
	_Vec2(const _Vec2& vect)
		:
		_Vec2(vect.x, vect.y)
	{}
	template <typename T2>
	explicit operator _Vec2<T2>() const
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
	_Vec2&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		return *this;
	}
	_Vec2	GetNormalized() const
	{
		_Vec2 norm = *this;
		norm.Normalize();
		return norm;
	}
	_Vec2	operator-() const
	{
		return _Vec2(-x, -y);
	}
	_Vec2&	operator=(const _Vec2 &rhs)
	{
		x = rhs.x;
		y = rhs.y;
		return *this;
	}
	_Vec2&	operator+=(const _Vec2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		return *this;
	}
	_Vec2&	operator-=(const _Vec2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
	T		operator*(const _Vec2 &rhs) const
	{
		return x * rhs.x + y * rhs.y;
	}
	_Vec2	operator+(const _Vec2 &rhs) const
	{
		return _Vec2(*this) += rhs;
	}
	_Vec2	operator-(const _Vec2 &rhs) const
	{
		return _Vec2(*this) -= rhs;
	}
	_Vec2&	operator*=(const T &rhs)
	{
		x *= rhs;
		y *= rhs;
		return *this;
	}
	_Vec2	operator*(const T &rhs) const
	{
		return _Vec2(*this) *= rhs;
	}
	_Vec2&	operator/=(const T &rhs)
	{
		x /= rhs;
		y /= rhs;
		return *this;
	}
	_Vec2	operator/(const T &rhs) const
	{
		return _Vec2(*this) /= rhs;
	}
	bool	operator==(const _Vec2 &rhs) const
	{
		return x == rhs.x && y == rhs.y;
	}
	bool	operator!=(const _Vec2 &rhs) const
	{
		return !(*this == rhs);
	}
public:
	T x;
	T y;
};

typedef _Vec2<float> Vecf2;
typedef _Vec2<double> Vecd2;
typedef _Vec2<int> Veci2;