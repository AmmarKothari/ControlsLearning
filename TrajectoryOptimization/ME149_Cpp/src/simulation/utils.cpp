#include "utils.h"

// TODO: Move these into a helper folder

// Cooler implementation: https://gist.github.com/mortenpi/f20a93c8ed3ee7785e65
template <typename T>
std::vector<float> linspace(T min, T max, int N){
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