#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>

class quaternion{
	public:
		quaternion();
		quaternion(float, float, float, float);
		void test();
		void set_xyzw(float, float, float, float);
		void normalize();
		float get_x();
		float get_y();
		float get_z();
		float get_w();
		void print();
		void print_test();
		friend std::ostream& operator<< (std::ostream &out, quaternion const& q);

	private:
		float x, y, z, w;
};

#endif