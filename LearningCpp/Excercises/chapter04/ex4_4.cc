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

	vector<double> base_double;
	vector<double> sq_double;

	double max_base_double = 1000;

	for (double i = 0; i <= max_base_double; ++i){
		base_double.push_back(i);
		sq_double.push_back(i*i);
	}
	
	int max_base_length = to_string(base_double.back()).length();
	int max_sq_length = to_string(sq_double.back()).length();
	for (int i = 0; i <= max_base_double; ++i){
		cout << setw(max_base_length) << base_double[i] << "  " << setw(max_sq_length) << sq_double[i] << endl;
	}
}