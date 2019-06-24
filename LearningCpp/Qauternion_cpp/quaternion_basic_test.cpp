// A basic test for quaternion operations
#include "quaternion.h"

int test_create_quaternion() {
	quaternion q;
	q.set_xyzw(0, 0, 0, 1);
	// printf("Setting quat to <.4f, .4f, .4f, .4f>", q.get_x(), q.get_y(), q.get_z(), q.get_w())
	return 1;
};




int main() {
	test_create_quaternion();
	return 1;
};
