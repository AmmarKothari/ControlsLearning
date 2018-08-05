
#include <string>
#include <iostream>

using namespace std;
class Test1 {
public:
	string static getString() {
		return "Test";
	}

	int static getExpectedAnswer() {
		return 4*3*2*1;
	}

	int static getRunO() {
		return 4*4+4*3*2*1;
	}
};

void permutation(string str, string prefix) {
	if (str.length() == 0) {
		cout << prefix << endl;
	}
	else {
		for (int i=0; i<str.length(); i++) {
			string rem = str.substr(0,i) + str.substr(i+1);
			permutation(rem, prefix+str[i]);
		}
	}
}

void permutation(string str) {
	permutation(str, "");
}


void runTest1() {
	string test_string = Test1::getString();
	permutation(test_string);
	int answer = Test1::getExpectedAnswer();
	int order = Test1::getRunO();
	printf("Test String: %s, String Length: %d, Answer: %d, RunTime Order: %d\n", test_string.c_str(), test_string.size(), answer, order);
}




int main() {
	runTest1();
	return 0;
}