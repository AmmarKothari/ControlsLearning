#ifndef GUARD_ex4_utils_h
#define GUARD_ex4_utils_h

#include <vector>
#include <string>

std::vector<std::string> store_input();
void print_input(std::vector<std::string>);
void print_input_length(std::vector<std::string>);
int longest_word(std::vector<std::string>);
std::vector<std::string> unique_words(std::vector<std::string>);
std::vector<int> unique_count(std::vector<std::string>, std::vector<std::string>);

#endif
