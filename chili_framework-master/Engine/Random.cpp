#include "Random.h"

Random::Random(uint_least32_t seed = time(NULL))
{
	rng = std::mt19937(seed);
	d_dist = std::uniform_real_distribution<double_t>();
}

int32_t Random::next(int32_t minval = std::numeric_limits<int32_t>::min(), int32_t maxval = std::numeric_limits<int32_t>::max())
{
	return i_dist(rng);
}

double Random::next_double(double minval = std::numeric_limits<double>::min(), double maxval = std::numeric_limits<double>::max())
{
	return d_dist(rng);
}