// https://eigen.tuxfamily.org/dox/group__TutorialMatrixClass.html

#include <iostream>
#include <Eigen/Dense>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

using namespace Eigen;
using namespace std;

void initializeVectors() {

	cout << "initialize vector with 2 rows with constants" << endl;
	Vector2d a(5.0, 6.0);
	cout << a << endl;


	cout << "initialize vector with 3 rows with constants" << endl;
	Vector3d b(5, 6.0, 7.0);
	cout << b << endl;


	cout << "initialize vector with 4 rows with constants" << endl;
	Vector4d c(5, 6.0, 7.0, 8.0);
	cout << c << endl;

	cout << "Initialize a N dimensional vector of zeros" << endl;
	VectorXf d(5);
	cout << d << endl;

	cout << "Initialize a N dimensional vector of values with Comma Initializer" << endl;
	Vector4f e;
	e << 1,2,3,4;
	/*
	can't do
	e << 1;
	e << 2;
	*/
	cout << e << endl;

	cout << "Initialize a vector of values with random initializer" << endl;
	Vector2f f = Vector2f::Random();
	cout << f << endl;
}

void initializeMatrix() {
	cout << "Initialize a NxM dimensional matrix of no values" << endl;
	MatrixXf b;
	cout << b << endl;

	cout << "Initialize a NxM dimensional matrix of zeros" << endl;
	MatrixXf a(3,2);
	cout << a << endl;

	cout << "Initialize a NxM dimensional matrix of values with Comma Initializer" << endl;
	Matrix3f m;
	m << 1, 2, 3,
	     4, 5, 6,
	     7, 8, 9;
	cout << m << endl;

	cout << "Initialize a matrix of values with random initializer" << endl;
	srand((unsigned int) time(0));
	Matrix2f f = Matrix2f::Random();
	cout << f << endl;

	cout << "Index of max and min values" << endl;
	ptrdiff_t i, j;
	float minval = f.minCoeff(&i,&j);
	cout << "Its minimum coefficient (" << minval 
	<< ") is at position (" << i << "," << j << ")\n\n";
}


void matrixMultiplication() {
	Matrix3f a;
	a << 1,2,3,
		1,2,3,
		1,2,3;
	MatrixXf b(3,1);
	b << 1,2,3;
	
	cout << "a" << endl;
	cout << a << endl;

	cout << "b" << endl;
	cout << b << endl;

	cout << "a * b" << endl;
	cout << a*b <<endl;

	cout << "b.T * a" << endl;
	cout << b.transpose()*a <<endl;
}


void blockOperations() {
	Matrix3f a;
	a << 1,2,3,
		4,5,6,
		7,8,0;
	cout << a << endl;
	MatrixXf b = a.bottomRightCorner(2,2);
	Block<Matrix3f> sub = a.bottomRightCorner(2,2);
	cout << "Bottom Right Block: " << endl;
	cout << b << endl;
	cout << sub << endl;
	a(2,2) = 10;
	cout << a << endl;
	cout << "Bottom Right Block: " << endl;
	cout << b << endl;
	cout << sub << endl;

}


int main() {

	initializeVectors();

	initializeMatrix();

	matrixMultiplication();

	blockOperations();

}