#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include <vector>
#include <boost/python.hpp>

namespace py = boost::python;

class quaternion{
	public:
		quaternion();
		quaternion(float, float, float, float);
		void set_xyzw(float, float, float, float);
		float get_x();
		float get_y();
		float get_z();
		float get_w();
		std::vector<float> get_xyzw();

		void normalize();
		quaternion get_conjugate();

		void print();
		void print_test();
		void test();


		friend std::ostream& operator<< (std::ostream &out, quaternion const& q);

	private:
		float x, y, z, w;
};

#endif