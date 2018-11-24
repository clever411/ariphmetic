#ifndef DEFS_HPP
#define DEFS_HPP

#include <random>
#include "ShuffleRandom.hpp"

#define CLEAR_CONSOLE system("clear");





typedef std::default_random_engine random_engine_type;
typedef clever::ShuffleRandom< random_engine_type > distribution_type;





#endif
