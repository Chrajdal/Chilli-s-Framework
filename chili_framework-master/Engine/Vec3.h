#pragma once

#include "Vec2.h"

template <typename T>
class Cvector3 : public Cvector2<T>
{
public:
	Cvector3() {}
	Cvector3( T x,T y,T z )
		:
		Cvector2( x,y ),
		z( z )
	{}
	Cvector3( const Cvector3& vect )
		:
		Cvector3( vect.x,vect.y,vect.z )
	{}
	template <typename T2>
	explicit operator Cvector3<T2>() const
	{
		return{ (T2)x,(T2)y,(T2)z };
	}
	T LenSq() const
	{
		return sq( *this );
	}
	T Len() const
	{
		return sqrt( LenSq() );
	}
	Cvector3&	Normalize()
	{
		const T length = Len();
		x /= length;
		y /= length;
		z /= length;
		return *this;
	}
	Cvector3	GetNormalized() const
	{
		Cvector3 norm = *this;
		norm.Normalize();
		return norm;
	}
	Cvector3	operator-() const
	{
		return Cvector3( -x,-y,-z );
	}
	Cvector3&	operator=( const Cvector3 &rhs )
	{
		x = rhs.x;
		y = rhs.y;
		z = rhs.z;
		return *this;
	}
	Cvector3&	operator+=( const Cvector3 &rhs )
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}
	Cvector3&	operator-=( const Cvector3 &rhs )
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}
	T		operator*( const Cvector3 &rhs ) const
	{
		return x * rhs.x + y * rhs.y + z * rhs.z;
	}
	Cvector3	operator+( const Cvector3 &rhs ) const
	{
		return Cvector3( *this ) += rhs;
	}
	Cvector3	operator-( const Cvector3 &rhs ) const
	{
		return Cvector3( *this ) -= rhs;
	}
	Cvector3&	operator*=( const T &rhs )
	{
		x *= rhs;
		y *= rhs;
		z *= rhs;
		return *this;
	}
	Cvector3	operator*( const T &rhs ) const
	{
		return Cvector3( *this ) *= rhs;
	}
	Cvector3	operator%( const Cvector3& rhs ) const
	{
		return Cvector3(
			y * rhs.z - z * rhs.y,
			z * rhs.x - x * rhs.z,
			x * rhs.y - y * rhs.x );
	}
	Cvector3&	operator/=( const T &rhs )
	{
		x /= rhs;
		y /= rhs;
		z /= rhs;
		return *this;
	}
	Cvector3	operator/( const T &rhs ) const
	{
		return Cvector3( *this ) /= rhs;
	}
	bool	operator==( const Cvector3 &rhs ) const
	{
		return x == rhs.x && y == rhs.y && rhs.z = z;
	}
	bool	operator!=( const Cvector3 &rhs ) const
	{
		return !(*this == rhs);
	}
public:
	T z;
};