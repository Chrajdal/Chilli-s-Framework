#include "utils.h"

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

long sq_distance(const Cvector2<int> & a, const Cvector2<int> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
long long sq_distance(const Cvector2 <long> & a, const Cvector2<long> & b)
{
	return (long)((a.m_x - b.m_x) * (long)(a.m_x - b.m_x))
		+ (long)((a.m_y - b.m_y) * (long)(a.m_y - b.m_y));
}
double sq_distance(const Cvector2<float> & a, const Cvector2<float> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
double sq_distance(const Cvector2<double> & a, const Cvector2<double> & b)
{
	return (double)((a.m_x - b.m_x) * (double)(a.m_x - b.m_x))
		+ (double)((a.m_y - b.m_y) * (double)(a.m_y - b.m_y));
}
void gen_random(string & str, const int len)
{
	str = "";
	static const char alphanum[] =
		"0123456789"
		"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"abcdefghijklmnopqrstuvwxyz";

	for (int i = 0; i < len; ++i)
		str += alphanum[rand() % (sizeof(alphanum) - 1)];
}
void gen_randascii(string & str, const int len)
{
	str = "";
	static const char arr[] = "!\"#$%&\'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
	for (int i = 0; i < len; ++i)
		str += arr[rand() % (sizeof(arr) - 1)];
}

//-----------------------------------------------------------------------------
