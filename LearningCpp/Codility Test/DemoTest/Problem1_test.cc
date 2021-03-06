#include <algorithm>
#include <vector>
#include <ios>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "Problem1.h"
// #include "test.h"

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;

int main() {
	vector<int> A;
	vector<int> &B = A;
	int val;
	string text_line = "";
	while (true) {
		// get array as string
		getline(cin, text_line);

		if (text_line == "END") {
			// end of test cases
			cout << "End of Test" << endl;
			break;
		};

		// convert to array
		stringstream myStream(text_line);
		while (myStream >> val){
			A.push_back(val);
		};
		
		// get solution from program
		B = A;
		int sol;
		sol = solution(B);
		cout << "Solution: " << sol << endl;
		
		// get next line of text file
		getline(cin, text_line);

		// get answer from text line
		string temp;
		int answer;
		stringstream myStream2(text_line);
		while (!myStream2.eof()) {
			myStream2 >> temp;

			if (stringstream(temp) >> answer) {
				continue;
			};
			temp = "";
		};
		cout << "Answer: " << answer << endl;
		
		// Decide whether result is correct
		if (sol == answer){
			cout << "PASS";
		} else {
			cout << "FAIL";
		};
		cout << endl;
		cout << endl;

		A.clear();
	}
	return 0;



}