
#include <algorithm>
#include <vector>
#include <ios>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

// #include "Problem1.h"

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;


string replaceStrChar(string str, char replace, char ch) {
	int found = 0;
  // set our locator equal to the first appearance of any character in replace
  // size_t i = str.find_first_of(replace);

  while (found != (int)string::npos) { // While our position in the sting is in range.
    str[found] = ch; // Change the character at position.
    found = str.find_first_of(replace, found+1); // Relocate again.
  };

  return str; // return our new string.
};

int convert2secs(string in) {
	std::size_t comma_idx = in.find(',');
	string time_str = in.substr(0,comma_idx);
	time_str = replaceStrChar(time_str, ':', ' ');
	// int time;
	stringstream time_ss(time_str);
	int h;
	time_ss >> h;
	int m;
	time_ss >> m;
	int s;
	time_ss >> s;
	// string rem;
	// getline(time_ss, rem);
	// cout << "Remainder: " << rem << endl;
	// cout << "Hours: " << h << endl;
	// cout << "Mins: " << m << endl;
	// cout << "Secs: " << s << endl;

	int s_out = h*3600 + m*60 + s;


	// cout << "Time: " << s_out << endl;
	return s_out;

}



int getNumber(string str) {
	// returns the phone number as an int

	std::size_t comma_idx = str.find(',');
	string num_str = str.substr(comma_idx+1, string::npos);

	num_str.erase(std::remove(num_str.begin(), num_str.end(), '-'), num_str.end());
	stringstream ss(num_str);
	int num;
	ss >> num;

	return num;
}

int costOfCall(int time) {
	// return value in cents
	int cost;
	if (time < 300) {
		cost = 3*time;
	} else {
		int m;
		m = time/60; // convert to minutes
		cost = 150*m;
		if (m*60 - time != 0) {
			cost = cost + 150;
		}
	}
	return cost;
}






int solution(char *S) {
    // write your code in C99 (gcc 6.2.0)
    cout << "In Function" << endl;
    cout << S << endl;

    stringstream log;
    log << S;
    string single_entry;

    vector<int> costs;
    vector<int> times;
    vector<int> numbers;

    // get cost of each call, time, and number 
    while (true) {
    	if (log.str().find_first_of('\n', 0) != string::npos) {
    		getline(log, single_entry);

	    	if (single_entry.empty()) {
	    		break;
	    	}

    		int t_s = convert2secs(single_entry);
    		int c = costOfCall(t_s);
    		int num = getNumber(single_entry);
    		cout << single_entry << endl;
    		// cout << "Length: " << t_s << ", Cost: " << c << ", Number: " << num << endl;

    		std::vector<int>::iterator num_it = std::find(numbers.begin(), numbers.end(), num);
    		int num_index = std::distance(numbers.begin(), num_it);
    		if (num_it != numbers.end()) {
    			costs[num_index] += c;
    			times[num_index] += t_s;
    		} else {
	    		costs.push_back(c);
	    		times.push_back(t_s);
	    		numbers.push_back(num);
	    	}
    		// cout << "Length: " << times[num_index] <<  ", Cost: " << costs[num_index] << ", Number: " << num << endl;
	    	t_s = 0;
	    	c = 0;
	    	num = 0;


    	}
    }

    // find the largest number of minutes and set cost to zero
    std::vector<int>::iterator max_element = std::max_element(times.begin(), times.end());
    int max_idx = std::distance(times.begin(), max_element);
    int max_num = numbers[max_idx];

    std::vector<int>::iterator check_element = max_element;
    int check_element_idx = 0;
    while (true) { // check until the end
    	check_element = std::find(check_element+1, times.end(), times[max_idx]);
    	if (check_element == times.end()) {
    		break;
    	};
    	check_element_idx = std::distance(times.begin(), check_element);
    	if (numbers[check_element_idx] > max_num) {
    		max_num = numbers[check_element_idx];
    		max_idx = check_element_idx;
    	};

    };

    costs[max_idx] = 0;

    for (int i = 0; i < (int)costs.size(); i++) {
    	cout << "Length: " << times[i] <<  ", Cost: " << costs[i] << ", Number: " << numbers[i]<< endl;
    };

    // sum all the costs up
    int total_cost = 0;
    for (int i = 0; i < (int)costs.size(); i++) {
    	total_cost += costs[i];
    };

    cout << "Total Costs: " << total_cost << endl;

    return total_cost;
}