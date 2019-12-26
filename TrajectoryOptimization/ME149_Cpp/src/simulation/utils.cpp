#ifndef _GUARD_SIM_UTILS
#define _GUARD_SIM_UTILS

#include <vector>
// TODO: Move these into a helper folder
// TODO: write tests for these!

// Cooler implementation: https://gist.github.com/mortenpi/f20a93c8ed3ee7785e65
std::vector<float> linspace(float min, float max, size_t N){
    std::vector<float> out_linspace;
    float delta = (max - min) / float(N-1);
    for (int i=0; i<N; i++){
        out_linspace.push_back(min + i*delta);
    }
    return out_linspace;
}

std::vector<float> arange(float min, float max, float step_size){
    std::vector<float> out_range;
    out_range.push_back(min);
    while (out_range.back() <= max){
        out_range.push_back(out_range.back() + step_size);
    }
    return out_range;
}

#endif