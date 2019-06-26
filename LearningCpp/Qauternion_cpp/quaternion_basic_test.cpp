// A basic test for quaternion operations
#include "quaternion.h"
#include "color.h"
#include <iostream>

void print_pass(){
	printf("%sPASS%s ", GREEN, RESET);
}

void print_fail(){
	printf("%sFAIL%s ", RED, RESET);
}

void test_create_init_quaternion() {
	quaternion q1;
	printf("Q1: ");
	q1.set_xyzw(0.0, 0.0, 0.0, 1.0);
	q1.print_test();

	quaternion q2(0.0, 0.0, 0.0, 1.0);
	printf("Q2: ");
	q2.print_test();

	quaternion q3;
	printf("Q3: ");
	q3.print_test();

	quaternion q4(1.0, 0.0, 0.0, 0.0);
	printf("Q4: ");
	std::cout << q4 << std::endl;
	return;
};

void test_normalize_quaternion() {
	quaternion q1;
	q1.set_xyzw(1.0, 1.0, 1.0, 1.0);
	if (q1.get_x() == 0.5 and q1.get_y() == 0.5 and q1.get_z() == 0.5, q1.get_w() == 0.5){
		print_pass();	
	}
	else{
		print_fail();
	}
	printf("Q1: ");
	q1.print_test();

	quaternion q2(-1.0, 4.0, 2.0, 1.0);
	printf("Q2: ");
	q2.print_test();
}



int main() {
	test_create_init_quaternion();
	test_normalize_quaternion();
	return 0;
};
