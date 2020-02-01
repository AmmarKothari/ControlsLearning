#ifndef _GUARD_SIM_UTILS
#define _GUARD_SIM_UTILS

#include <vector>

template <typename T>
std::vector<float> linspace(T, T, int);
std::vector<float> arange(float, float, float);
#endif