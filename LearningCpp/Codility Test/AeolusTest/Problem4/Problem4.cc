
#include <algorithm>
#include <vector>
#include <ios>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::sort;
using std::string;


int solution(int N) {
    // write your code in C++14 (g++ 6.2.0)
    vector<string> all_nums;
    vector<string> unique_nums;
    std::ostringstream stm ;
    stm << N;
	string N_str = stm.str();

	unique_nums.push_back(N_str); // add original string to possible list
	all_nums.push_back(N_str);

	// get all permutations of number
	sort(N_str.begin(), N_str.end());
	all_nums.push_back(N_str);
	// cout << "String: " << N_str << endl;
	while (std::next_permutation(N_str.begin(), N_str.end())) {
		all_nums.push_back(N_str);

	}

	int num_possibilties = 0;
	string cur_string;
	for (int i = 0; i<all_nums.size(); i++){
		// cout << "String: " << all_nums[i] << endl;
		cur_string = all_nums[i];
		// cur_string[0] == "0")
		if (!all_nums[i].compare(0,1,"0")){ // skip if number starts with zero
			// cout << "Leading Zero: " << all_nums[i] << endl;
			continue;
		}
		// only add if it is a unique number (no swapping the same number)
		if (std::find(unique_nums.begin(), unique_nums.end(), all_nums[i]) == unique_nums.end()){
			unique_nums.push_back(all_nums[i]);
		}
		cur_string.clear();
	}

	int unique_counter = 0;
	for (int i = 0; i<unique_nums.size(); i++){
		// cout << "Unique: " << unique_nums[i] << endl;
		unique_counter += 1;
	}



    return unique_counter;
    
}