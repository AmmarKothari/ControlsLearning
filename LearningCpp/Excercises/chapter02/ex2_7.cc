#include <iostream>
#include <string>

// say what standard-library names we use
using std::cin;         using std::endl;
using std::cout;        using std::string;

/*
GOAL: 	count down from 10 to -5
*/

int main()
{
	int start = 10;
	int end = -5;
	int range = start - end;


	for (int i = 0; i != range; i++) {
		cout << start - i << endl;
	}
}