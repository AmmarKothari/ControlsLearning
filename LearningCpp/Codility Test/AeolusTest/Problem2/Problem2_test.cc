#include <algorithm>
#include <vector>
#include <ios>
#include <iomanip>
#include <iostream>
#include <string>
#include <sstream>

#include "Problem2.h"

using std::cin;
using std::cout;
using std::vector;
using std::endl;
using std::string;
using std::getline;
using std::stringstream;

int main() {
	vector<int> A;
	vector<vector<int> > A_mat;
	vector<vector<int> > &B = A_mat;
	int val;
	string text_line = "";
	string answer_ID = "Answer";
	string endTest_ID = "END";
	int answer_flag = true; 
	while (true) {
		// get array as string
		getline(cin, text_line);

		if (text_line == endTest_ID) {
			// end of test cases
			cout << "End of Test" << endl;
			break;
		};

		// go through inputs until you see answer
		while (answer_flag) {
			// cout << text_line << endl;
			if (text_line.find(answer_ID) != string::npos){
				std::cout << "found!" << endl;
				answer_flag = false;
				break;
			} else {
				// convert to array
				stringstream myStream(text_line);
				while (myStream >> val){
					A.push_back(val);
				};
				A_mat.push_back(A);
				A.clear();

				// cout << text_line << endl;
				// A.append(text_line);
				// A.append("\n");
			};
			getline(cin, text_line);
		};
		
		// get solution from program
		B = A_mat;
		int sol;
		sol = solution(B);
		cout << "Solution: " << sol << endl;
		
		// get next line of text file
		// getline(cin, text_line);

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

		A_mat.clear();
	}
	return 0;



}