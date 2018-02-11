#include <iostream>
#include <vector>
#include <iomanip>
#include <string>

using std::cout; using std::endl;
using std::vector;
using std::setw;
using std::setprecision;
using std::streamsize;
using std::string;
using std::to_string;


int main() {
	cout << "Print squares of integers between 0 and 100." << endl;

	vector<int> base_int;
	vector<int> sq_int;

	int max_base_int = 1000;

	for (int i = 0; i <= max_base_int; ++i){
		base_int.push_back(i);
		sq_int.push_back(i*i);
	}
	
	int max_base_length = to_string(base_int.back()).length();
	for (int i = 0; i <= max_base_int; ++i){
		cout << setw(max_base_length) << base_int[i] << "  "  << sq_int[i] << endl;
	}
}