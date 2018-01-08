#include <iostream>
#include <string>

// say what standard-library names we use
using std::cin;         using std::endl;
using std::cout;        using std::string;

/*
GOAL: 	draw a square, rectangle with *
*/

int main()
{
	const int H = 5;
	const int W = 9;

	// Square
	for (int h = 0; h != H; h++) {
		for (int w = 0; w != H; w++) {
			if (h == 0 || w == 0 || h == H-1 || w == H-1)
				cout << "*";
			else
				cout << " ";
		}
		cout << endl;

	}
	cout << endl;




	// Rectangle
	for (int h = 0; h != H; h++) {
		for (int w = 0; w != W; w++) {
			if (h == 0 || w == 0 || h == H-1 || w == W-1)
				cout << "*";
			else
				cout << " ";
		}
		cout << endl;

	}
	cout << endl;


	// Triangle
	for (int h = 0; h != H; h++) {
		for (int w = 0; w != W; w++) {
			if (h == (H-w)-1 || h == H-1 || h == (W/2-w)) 
				cout << "*";
			else
				cout << " ";
		}
		cout << endl;

	}
	cout << endl;






}