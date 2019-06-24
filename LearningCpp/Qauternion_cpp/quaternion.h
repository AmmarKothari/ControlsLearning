#include <iostream>

class quaternion{
	public:
		void test();
		void set_xyzw(float, float, float, float);
		float get_x();
		float get_y();
		float get_z();
		float get_w();
		// std::ostream& operator<< (std::ostream, quaternion);

	private:
		float x, y, z, w;
};