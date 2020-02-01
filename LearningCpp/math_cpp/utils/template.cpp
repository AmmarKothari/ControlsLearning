#include "template.h"


template <typename T>
std::vector<float> increasing_vector(T start, T end){
    std::vector<float> out;
    for (T i=start; i<=end; i++){
        out.push_back(float(i));
    }
    return out;
} 

template <typename T>
void print_vector(std::vector<T> in){
    for (auto &i: in){
        std::cout << i << ", ";
    }
    std::cout << std::endl;
}

int main(){
    std::vector<float> test;
    test = increasing_vector(int(1), int(10));
    print_vector(test);
    std::vector<float> test2;
    test2 = increasing_vector(float(1), float(10));
    print_vector(test2);
    return 0;
}