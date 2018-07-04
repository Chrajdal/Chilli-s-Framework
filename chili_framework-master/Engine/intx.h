#pragma once

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

intx fib(intx n);
intx fact(intx i);
intx two_to_x(intx x);
intx n_to_n(const intx & n);
string generatebignum(const intx & len);

//string date_string(void)
//{
//	auto t = std::time(nullptr);
//	auto tm = *std::localtime(&t);
//	stringstream s;
//	s << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
//	return s.str();
//}