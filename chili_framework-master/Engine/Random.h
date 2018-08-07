#pragma once

#include <random>
#include <ctime>

class Random
{
	std::mt19937 rng;
public:
	Random::Random(uint_least32_t seed = time(NULL))
	{
		rng = std::mt19937(seed);
	}

	int32_t Random::next(int32_t minval = std::numeric_limits<int32_t>::min(), int32_t maxval = std::numeric_limits<int32_t>::max())
	{
		std::uniform_int_distribution<int32_t> i_dist(minval, maxval);
		return i_dist(rng);
	}

	double_t Random::next_double(double_t minval = std::numeric_limits<double_t>::min(), double_t maxval = std::numeric_limits<double_t>::max())
	{
		std::uniform_real_distribution<double_t> d_dist(minval, maxval);
		return d_dist(rng);
	}

	float_t Random::next_float(float_t minval = std::numeric_limits<float_t>::min(), float_t maxval = std::numeric_limits<float_t>::max())
	{
		std::uniform_real_distribution<float_t> f_dist(minval, maxval);
		return f_dist(rng);
	}
};
