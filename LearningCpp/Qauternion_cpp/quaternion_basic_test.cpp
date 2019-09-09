// A basic test for quaternion operations
#include "quaternion.h"
#include "color.h"
#include <iostream>
#include <vector>
#include <boost/python.hpp>

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

void test_get_py_xyzw() {
	printf("Test - Get Python list\n");
	quaternion q1(1.0, 0.0, 0.0, 0.0);
	// boost::python::list py_list = q1.py_get_xyzw();
	q1.py_get_xyzw();
	// std::vector<float> value = boost::python::extract<std::vector<float>>(py_list.pop(0));                                                                              
  	// std::cout << value[0] << std::endl;
}

void test_get_xyzw_quaternion(){
	quaternion q;
	std::vector<float> xyzw = q.get_xyzw();
	if (xyzw[0] == 0.0 and xyzw[1] == 0.0 and xyzw[2] == 0.0 and xyzw[3] == 1.0){
		print_pass();
	}
	else {
		print_fail();
	}
	for (std::vector<float>::const_iterator i = xyzw.begin(); i != xyzw.end(); ++i){

    	printf("%.4f ", *i);
	};
	std::cout << std::endl;
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

void test_conjugate_quaternion() {
	quaternion q1;
	q1.set_xyzw(1.0, 1.0, 1.0, 1.0);
	quaternion q1_conj = q1.get_conjugate();
	if (q1_conj.get_x() == -0.5 and q1_conj.get_y() == -0.5 and q1_conj.get_z() == -0.5, q1_conj.get_w() == 0.5){
		print_pass();	
	}
	else{
		print_fail();
	}
	printf("Q1: ");
	q1_conj.print_test();
}


int main() {
	test_create_init_quaternion();
	test_normalize_quaternion();
	test_conjugate_quaternion();
	test_get_xyzw_quaternion();
	test_get_py_xyzw();
	return 0;
};
