#include "intx.h"

intx::intx(void)
	: sign(true)
{}
intx::~intx(void)
{}
intx::intx(const intx & src)
	: v(src.v), sign(src.sign)
{}
intx::intx(int _src)
	:
	sign(true)
{
	if (_src < 0)
	{
		sign = false;
		_src = -_src;
	}
	else if (_src == 0)
	{
		v.push_back(0);
		return;
	}
	ulli src = _src;
	while (src > 0)
	{
		v.insert(v.begin(), (src % C));
		src /= C;
	}
}
intx::intx(unsigned long long int src)
	: sign(true)
{
	while (src > 0)
	{
		v.push_front(src % C);
		src /= C;
	}
}
intx::intx(const char * src)
{
	string _src = src;
	if (_src.empty())
		throw CException("'intx & operator = (const char *)' call with an empty char array");
	*this = intx(_src);
}
intx::intx(const string & src)
	: sign(true)
{
	string tmpsrc = src;
	if (src.empty())
		throw CException("'intx(const string &)' call with an empty string");
	if (src[0] == '-')
		sign = false;
	while (!tmpsrc.empty())
	{
		size_t j = 0;
		string tmp = "";
		while (j < xC)
		{
			if (tmpsrc.empty())
				break;
			tmp.insert(tmp.begin(), *tmpsrc.rbegin());
			tmpsrc.pop_back();
			++j;
		}
		v.push_front(atoi(tmp.c_str()));
	}
}
intx & intx::operator = (const intx & src)
{
	if (this == &src)
		return *this;
	v = src.v;
	sign = src.sign;
	return *this;
}
intx & intx::operator = (int _src)
{
	v.clear();
	sign = true;
	if (_src < 0)
	{
		sign = false;
		_src = -_src;
	}
	else if (_src == 0)
	{
		v.push_back(0);
		return *this;
	}
	ulli src = _src;
	while (src > 0)
	{
		v.push_front(src % C);
		src /= C;
	}
	return *this;
}
intx & intx::operator = (unsigned long long int src)
{
	v.clear();
	sign = true;
	while (src > 0)
	{
		v.push_front(src % C);
		src /= C;
	}
	return *this;
}
intx & intx::operator = (const char * src)
{
	string _src = src;
	if (_src.empty())
		throw CException("'intx & operator = (const char *)' call with an empty char array");
	*this = intx(_src);
	return *this;
}
intx & intx::operator = (const string & src)
{
	v.clear();
	sign = true;
	string tmpsrc = src;
	if (src.empty())
		throw CException("'intx & operator =(const string &)' call with an empty string");
	if (src[0] == '-')
		sign = false;

	while (!tmpsrc.empty())
	{
		size_t j = 0;
		string tmp = "";
		while (j < xC)
		{
			if (tmpsrc.empty())
				break;
			tmp.insert(tmp.begin(), *tmpsrc.rbegin());
			tmpsrc.pop_back();
			++j;
		}
		v.push_front(atoi(tmp.c_str()));
	}
	return *this;
}
intx intx::operator - () const
{
	intx res(*this);
	if (res.sign == true && res != 0)
	{
		res.sign = false;
	}
	else
	{
		res.sign = true;
	}
	return res;
}
intx intx::operator + () const
{
	intx res(*this);
	res.sign = true;
	return res;
}
intx operator + (const intx & a, const intx & b)
{
	if (a == 0)
		return b;
	if (b == 0)
		return a;

	intx res;
	if (a.v.size() < b.v.size())
	{
		ulli carry = 0;
		deque<ulli>::const_reverse_iterator j = b.v.crbegin();
		for (deque<ulli>::const_reverse_iterator i = a.v.crbegin(); i != a.v.crend(); ++i, ++j)
		{
			ulli tmp = *i + *j + carry;
			carry = 0;

			if (tmp >= intx::C)
			{
				carry = 1;
				tmp -= intx::C;
			}
			res.v.push_front(tmp);
		}

		for (; j != b.v.crend(); ++j)
		{
			ulli tmp = *j + carry;
			carry = 0;
			if (tmp >= intx::C)
			{
				carry = 1;
				tmp -= intx::C;
			}
			res.v.push_front(tmp);
		}
	}
	else if (a.v.size() > b.v.size())
	{
		return (b + a);
	}
	else // a.v.size() == b.v.size()
	{
		ulli carry = 0;
		deque<ulli>::const_reverse_iterator j = b.v.crbegin();
		deque<ulli>::const_reverse_iterator i = a.v.crbegin();
		for (; i != a.v.crend(); ++i, ++j)
		{
			ulli tmp;
			tmp = *i + *j + carry;
			carry = 0;
			if (tmp >= intx::C)
			{
				carry = 1;
				tmp -= intx::C;
			}
			res.v.push_front(tmp);
		}
		if (carry == 1)
			res.v.push_front(carry);
	}
	return res;
}
intx operator - (const intx & a, const intx & b)
{
	if (b == 0)
		return a;
	if (a == 0)
		return -b;

	if (b < 0)
	{
		if (a < 0) // -x - -y
			return -((-a) - (-b));
		else // +x - -y
			return (a + (-b));
	}
	else
	{
		if (a < 0) // -x - +y
			return -((-a) + b);
		else {} // +x - +y
				// continue to calculate the difference;
	}

	if (a > b)
	{
		ulli carry = 0;
		ulli tmp = 0;
		intx result;
		deque<ulli>::const_reverse_iterator i = a.v.crbegin();
		deque<ulli>::const_reverse_iterator j = b.v.crbegin();
		for (; j != b.v.crend(); ++i, ++j)
		{
			if (*i < (*j + carry))
			{
				tmp = (intx::C + *i) - (*j + carry);
				carry = 1;
			}
			else
			{
				tmp = *i - *j - carry;
				carry = 0;
			}
			result.v.push_front(tmp);
		}
		for (; i != a.v.crend(); ++i)
		{
			if (*i < carry)
			{
				tmp = intx::C + *i - carry;
				carry = 1;
			}
			else
			{
				tmp = *i - carry;
				carry = 0;
			}
			result.v.push_front(tmp);
		}

		while (*(result.v.begin()) == 0 && !result.v.empty())
			result.v.pop_front();
		return result;
	}
	else if (a < b)
		return -((b - a));
	else // *this == src
		return intx(0);
}
intx intx::basic_multiply(const intx & a, const intx & b)
{
	// O(c * n^2)
	if (a == 0 || b == 0)
		return 0;
	intx result;
	ulli counter1 = 0;
	ulli counter2 = 0;
	for (deque<ulli>::const_reverse_iterator i = a.v.crbegin(); i != a.v.crend(); ++i)
	{
		for (deque<ulli>::const_reverse_iterator j = b.v.crbegin(); j != b.v.crend(); ++j)
		{
			ulli tmp;
			tmp = (*i) * (*j);
			intx mez = tmp;
			for (ulli k = 0; k < (counter2 + counter1); ++k)
				mez.v.push_back(0);
			counter1++;
			result += mez;
		}
		counter1 = 0;
		counter2++;
	}
	return result;
}
intx operator * (const intx & a, const intx & b)
{
	if (a == 0 || b == 0)
		return 0;
	if (a == 1)
		return b;
	else if (a == -1)
		return -b;
	if (b == 1)
		return a;
	else if (b == -1)
		return -a;

	bool result_sign = a.sign * b.sign;

	// Karatsuba ~ O(n^1.5849)
	int N = max(a.v.size(), a.v.size());
	if (N > 16)
	{
		unsigned int half_size = N / 2;

		// splitting numbers:
		intx k;
		intx l;
		intx m;
		intx n;

		if (a.v.size() <= half_size)
		{
			k = 0;
			l = a;
		}
		else
		{
			deque<ulli> q(a.v.begin(), a.v.end() - half_size);
			deque<ulli> r(a.v.end() - half_size, a.v.end());
			k.v = q;
			l.v = r;
		}

		if (b.v.size() <= half_size)
		{
			m = 0;
			n = b;
		}
		else
		{
			deque<ulli> s(b.v.begin(), b.v.end() - half_size);
			deque<ulli> t(b.v.end() - half_size, b.v.end());
			m.v = s;
			n.v = t;
		}

		// removing leading zeros		
		while (!l.v.empty() && *l.v.begin() == 0)
			l.v.pop_front();
		if (l.v.empty())
			l = 0;

		while (!n.v.empty() && *n.v.begin() == 0)
			n.v.pop_front();
		if (n.v.empty())
			n = 0;

		intx w2 = k * m;
		intx w0 = l * n;
		intx w1 = (k + l)*(m + n) - w2 - w0;

		for (unsigned i = 0; i < 2 * half_size; ++i)
			w2.v.push_back(0);
		for (unsigned i = 0; i < half_size; ++i)
			w1.v.push_back(0);

		intx result = w0 + w1 + w2;

		while (!result.v.empty() && *result.v.begin() == 0)
			result.v.pop_front();
		if (result.v.empty())
			result = 0;

		result.sign = result_sign;
		return result;
	}

	// O(c * n^2)
	intx result;
	ulli counter1 = 0;
	ulli counter2 = 0;
	for (deque<ulli>::const_reverse_iterator i = a.v.crbegin(); i != a.v.crend(); ++i)
	{
		for (deque<ulli>::const_reverse_iterator j = b.v.crbegin(); j != b.v.crend(); ++j)
		{
			ulli tmp;
			tmp = (*i) * (*j);
			intx mez = tmp;
			for (ulli k = 0; k < (counter2 + counter1); ++k)
				mez.v.push_back(0);
			counter1++;
			result += mez;
		}
		counter1 = 0;
		counter2++;
	}
	result.sign = result_sign;
	return result;
}
intx operator / (const intx & a, const intx & b)
{
	if (b == 0)
		throw CException("You can't divide by zero.");
	if (a == 0)
		return 0;
	if (b == 1)
		return a;
	if (b == -1)
		return -a;

	bool result_sign = a.sign * b.sign;

	intx x = a + b;
	x.sign = result_sign;
	return x;
}
intx operator % (const intx & a, const intx & b)
{
	return a + b;
}
intx operator ^ (const intx & a, const intx & b)
{
	return a + b;
}
intx operator | (const intx & a, const intx & b)
{
	return a + b;
}
intx operator & (const intx & a, const intx & b)
{
	return a + b;
}
intx operator << (const intx & a, const intx & b)
{
	return a + b;
}
intx operator >> (const intx & a, const intx & b)
{
	return a + b;
}
intx & intx::operator += (const intx & src)
{
	*this = (*this + src);
	return *this;
}
intx & intx::operator -= (const intx & src)
{
	*this = (*this - src);
	return *this;
}
intx & intx::operator ++ (void)
{
	*this = (*this + intx(1));
	return *this;
}
intx & intx::operator -- (void)
{
	*this = (*this - intx(1));
	return *this;
}
intx intx::operator ++ (int) const
{
	return intx(*this + intx(1));
}
intx intx::operator -- (int) const
{
	return intx(*this - intx(1));
}
intx & intx::operator *= (const intx & src)
{
	*this = (*this * src);
	return *this;
}
intx & intx::operator /= (const intx & src)
{
	*this = (*this / src);
	return *this;
}
intx & intx::operator %= (const intx & src)
{
	*this = (*this % src);
	return *this;
}
bool intx::operator > (const intx & rhs) const
{
	if (sign == true && rhs.sign == false)
		return true;
	else if (sign == false && rhs.sign == true)
		return false;
	else // sing == rhs.sign
	{
		if (sign == true)
		{
			if (v.size() > rhs.v.size())
				return true;
			else if (v.size() < rhs.v.size())
				return false;
			else // v.size() == rhs.v.size()
			{
				deque<ulli>::const_iterator j = rhs.v.begin();
				for (deque<ulli>::const_iterator i = v.begin(); i != v.end(); i++, j++)
				{
					if (*i > *j)
						return true;
					else if (*i < *j)
						return false;
					else
						continue;
				}
				return false;
			}
		}
		else // sign == false
			return -intx(*this) < (-rhs);
	}
}
bool intx::operator <  (const intx & rhs) const
{
	return rhs > *this;
}
bool intx::operator <= (const intx & rhs) const
{
	return !(*this > rhs);
}
bool intx::operator >= (const intx & rhs) const
{
	return !(*this < rhs);
}
bool intx::operator == (const intx & rhs) const
{
	if (v.size() != rhs.v.size() || sign != rhs.sign)
		return false;
	for (unsigned int i = 0; i < v.size(); ++i)
		if (v[i] != rhs.v[i])
			return false;
	return true;
}
bool intx::operator != (const intx & rhs) const
{
	return !(*this == rhs);
}
ostream & operator<<(ostream & os, const intx & src)
{
	if (src.sign == false)
		os << "-";
	if (src == (intx(0)))
	{
		os << "0";
		return os;
	}
	for (auto i : src.v)
	{
		if (i != *(src.v.begin()))
			os << setw(src.xC) << setfill('0');
		os << i;
	}
	return os;
}
istream & operator>>(istream & is, intx & src)
{
	string line;
	string _num;
	while (getline(is, line))
	{
		if (line.empty())
			break;
		_num += line;
	}
	src = intx(_num);
	return is;
}
string intx::toString(void) const
{
	ostringstream os;
	if (sign == false)
		os << "-";
	if (*this == (intx(0)))
	{
		os << "0";
		return os.str();
	}
	for (auto i : v)
	{
		if (i != *(v.begin()))
			os << setw(xC) << setfill('0');
		os << i;
	}
	return os.str();
}