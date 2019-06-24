#include <iostream>
#include "quaternion.h"

void quaternion::test(){
	std::cout << "Testing if this works!" << std::endl;
};

void quaternion::set_xyzw(float x_in, float y_in, float z_in, float w_in){
	x = x_in;
	y = y_in;
	z = z_in;
	w = w_in;
	std::cout << this;
}

// std::ostream& operator<< (std::ostream &out, quaternion const& q){
// 	char buffer [100];
// 	sprintf(buffer, "Setting quat to <.4f, .4f, .4f, .4f>", x, y, z, w);
// 	out << buffer;
// }

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


int main()
{
	quaternion q;
	q.test();
}