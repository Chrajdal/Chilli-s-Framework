#pragma once

#include <random>
#include <ctime>

class Random
{
	std::mt19937 rng;
	std::uniform_int_distribution<int32_t> i_dist;
	std::uniform_real_distribution<double_t> d_dist;
public:
	Random(uint_least32_t seed = time(NULL));
	~Random() {}

	int32_t next(int32_t minval = std::numeric_limits<int32_t>::min(), int32_t maxval = std::numeric_limits<int32_t>::max());
	double_t next_double(double_t minval = std::numeric_limits<double_t>::min(), double_t maxval = std::numeric_limits<double_t>::max());
};
