#include <iostream>
#include "quaternion.h"
#include <math.h>
#include <vector>
#include <boost/python/numpy.hpp>
#include <boost/python.hpp>

namespace py = boost::python;

void quaternion::test(){
	std::cout << "Testing if this works!" << std::endl;
};

void quaternion::set_xyzw(float x_in, float y_in, float z_in, float w_in){
	x = x_in;
	y = y_in;
	z = z_in;
	w = w_in;
	this->normalize();
};

std::vector<float> quaternion::get_xyzw(){
	std::vector<float> xyzw_vec;
	xyzw_vec.push_back(x);
	xyzw_vec.push_back(y);
	xyzw_vec.push_back(z);
	xyzw_vec.push_back(w);
	return xyzw_vec;
};

void quaternion::print(){
	printf("Quaternion <%.4f, %.4f, %.4f, %.4f>\n", this->get_x(), this->get_y(), this->get_z(), this->get_w());
}

void quaternion::print_test(){
	// use this to compare outsputs during testing
	printf("%.4f, %.4f, %.4f, %.4f\n", this->get_x(), this->get_y(), this->get_z(), this->get_w());
}

quaternion::quaternion(){
	this->set_xyzw(0.0, 0.0, 0.0, 1.0);
}

quaternion::quaternion(float x_in, float y_in, float z_in, float w_in){
	this->set_xyzw(x_in,y_in,z_in,w_in);
}

void quaternion::normalize(){
	// normalizes the quaternion
	float d;
	d = pow(x*x + y*y + z*z + w*w, 0.5);
	x = x/d;
	y = y/d;
	z = z/d;
	w = w/d;
}

quaternion quaternion::get_conjugate(){
	return quaternion(-x, -y, -z, w);
}

std::ostream& operator<< (std::ostream &out, quaternion const& q){
	char buffer [100];
	sprintf(buffer, "Quaternion <%.4f, %.4f, %.4f, %.4f>", q.x, q.y, q.z, q.w);
	out << buffer;
	return out;
}

float quaternion::get_x(){
	return x;
};
float quaternion::get_y(){
	return y;
};
float quaternion::get_z(){
	return z;
};
float quaternion::get_w(){
	return w;
};

// int main()
// {
// 	quaternion q1;
// 	q1.test();
// 	q1.set_xyzw(0.0, 0.0, 0.0, 1.0);
// 	q1.print();
// 	quaternion q2;
// 	q2.print();
// }