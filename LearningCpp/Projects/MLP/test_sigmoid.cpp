
#include <vector>
#include <iostream>

#include "MLP.h"


using namespace std;


int main() {
	MLP mlp(2, 10, 1);
	vector<float> temp;
	vector<float> sig_out;
	for (float i = -10; i < 10; i+= 0.1)
	{
		temp.push_back(i-0.1);
		temp.push_back(i);
		temp.push_back(i+0.1);
		sig_out = mlp.sigmoid(temp);
		cout << "X: " << i << ", Sigmoid Value: ";
		mlp.printMatrix(sig_out);
		cout << endl;
		temp.clear();
		sig_out.clear();
	}
// for (int i=0; i<100000; i++) {
// 	// 
// }
}