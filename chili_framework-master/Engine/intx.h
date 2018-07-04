#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <iomanip>
#include <string>
#include <iomanip>
#include <cmath>
#include <sstream>
#include <vector>
#include <deque>
#include <algorithm>
#include <functional>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <cassert>
#include <sstream>
#include <ctime>
#include <numeric>
#include <thread>
#include <mutex>
#include <fstream>
#include <bitset>
#include "utils.h"

using namespace std;
using namespace std::chrono;

class CException
{
public:
	CException(const string & src)
		:
		name(src)
	{
	}
	~CException(void)
	{
	}
	friend ostream & operator << (ostream & os, const CException & src)
	{
		return os << "Caught exception: " << src.name;
	}
private:
	CException(void) {};
	string name;
};

typedef unsigned long long int ulli;

class intx
{
public:
	intx(void);
	intx(const intx & src);
	intx(int src);
	intx(unsigned long long int src);
	intx(const char * src);
	intx(const string & src);
	intx & operator = (const intx & src);
	intx & operator = (int x);
	intx & operator = (unsigned long long int src);
	intx & operator = (const char * src);
	intx & operator = (const string & src);
	~intx(void);

	friend intx operator + (const intx & a, const intx & b);
	friend intx operator - (const intx & a, const intx & b);
	friend intx operator * (const intx & a, const intx & b);
	friend intx operator / (const intx & a, const intx & b);
	friend intx operator % (const intx & a, const intx & b);

	friend intx operator ^ (const intx & a, const intx & b);
	friend intx operator | (const intx & a, const intx & b);
	friend intx operator & (const intx & a, const intx & b);
	friend intx operator << (const intx & a, const intx & b);
	friend intx operator >> (const intx & a, const intx & b);


	intx basic_multiply(const intx & a, const intx & b);

	intx operator - () const;
	intx operator + () const;
	intx & operator ++ (void);
	intx & operator -- (void);
	intx operator ++ (int) const;
	intx operator -- (int) const;

	intx & operator += (const intx & src);
	intx & operator -= (const intx & src);
	intx & operator *= (const intx & src);
	intx & operator /= (const intx & src);
	intx & operator %= (const intx & src);

	bool operator >  (const intx & rhs) const;
	bool operator <  (const intx & rhs) const;
	bool operator <= (const intx & rhs) const;
	bool operator >= (const intx & rhs) const;
	bool operator == (const intx & rhs) const;
	bool operator != (const intx & rhs) const;

	friend ostream & operator<<(ostream & os, const intx & src);
	friend istream & operator>>(istream & is, intx & src);
	string toString(void) const;

private:
	deque<ulli> v;
	bool sign;

	// this only works on 64bit architecture
	const static ulli C = 1000000000;
	const unsigned int xC = 9;
};

intx fib(intx n)
{
	if (n == 0)
		return 0;
	intx previous = 0;
	intx current = 1;
	for (intx i = 1; i < n; ++i)
	{
		intx next = previous + current;
		previous = current;
		current = next;
	}
	return current;
}

intx fact(intx i)
{
	intx result = 1;
	for (; i > 0; --i)
		result *= i;
	return result;
}

intx two_to_x(intx x)
{
	intx result = 2;
	for (intx i = 1; i < x; ++i)
		result *= 2;
	return result;
}

intx n_to_n(const intx & n)
{
	intx result = n;
	for (intx i = 1; i < n; ++i)
		result *= n;
	return result;
}

string generatebignum(const intx & len)
{
	string str;
	str.push_back((rand() % 9) + 1 + '0');
	for (intx i = 1; i < len; ++i)
		str.push_back((rand() % 10) + '0');
	return str;
}
//string date_string(void)
//{
//	auto t = std::time(nullptr);
//	auto tm = *std::localtime(&t);
//	stringstream s;
//	s << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
//	return s.str();
//}