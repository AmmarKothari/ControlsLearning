#ifndef QUATERNION_H
#define QUATERNION_H

#include <iostream>
#include <vector>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace py = boost::python;
namespace np = boost::python::numpy;

class quaternion{
	public:
		quaternion();
		quaternion(float, float, float, float);
		quaternion(float[4]);
		quaternion(float, float[3]);
		quaternion(float[3][3]);
		quaternion(float, float, float);
		void set_xyzw(float, float, float, float);
		float get_x();
		float get_y();
		float get_z();
		float get_w();
		std::vector<float> get_xyzw();

		void normalize();
		quaternion get_conjugate();
		quaternion global_rotation_by(quaternion &);
		quaternion relative_rotation_by(quaternion &);
		void rotate_point(float [], float []);

		void get_axis_angle(float &, float []);
		void get_rotation_matrix(float[3][3]);
		void get_euler_angles(float[3]);
		quaternion quat_mult(quaternion &);

		void print();
		// std::string quaternion::repr();

		quaternion(py::list);
		py::list py_get_xyzw();
		np::ndarray py_np_get_xyzw();
		void py_get_axis_angle(float &, py::list);


		friend std::ostream& operator<< (std::ostream &, quaternion const&);
		friend quaternion operator+ (quaternion &, quaternion &);
		friend quaternion operator- (quaternion &, quaternion &);

	private:
		float x, y, z, w;
};

#endif