#include <iostream>
#include "quaternion.h"
#include <math.h>
#include <vector>
#include <boost/python.hpp>
#include <boost/math/constants/constants.hpp>

#include <boost/python/numpy.hpp>


namespace py = boost::python;
namespace np = py::numpy;

#define pass (void)0
#define pi boost::math::constants::pi<float>
// Roughly inspired by: https://gist.github.com/shihyu/c5abf3ebff2f5f1cfd32a90968f04a3b

void quaternion::print(){
	printf("Quaternion <%.4f, %.4f, %.4f, %.4f>\n", this->get_x(), this->get_y(), this->get_z(), this->get_w());
}


// std::string quaternion::repr()
// {
//     std::stringstream s;
//     s << "C++ Quaternion ["<< this->get_xyzw() << "]";
//     return s.str();
// }

//* * * * * * * * * * * * * *
// INIT METHODS
//* * * * * * * * * * * * * * 
// empty constructor
quaternion::quaternion(){
	this->set_xyzw(0.0, 0.0, 0.0, 1.0);
}

// individual float constructor
quaternion::quaternion(float x_in, float y_in, float z_in, float w_in){
	this->set_xyzw(x_in,y_in,z_in,w_in);
}

// list of floats constructor
quaternion::quaternion(float q[4]) {
	this->set_xyzw(q[0], q[1], q[2], q[3]);
}

// axis angle constructor
quaternion::quaternion(float theta, float axis[3]){
	float x_in, y_in, z_in, w_in;
	x_in = axis[0] * sin(theta/2);
	y_in = axis[1] * sin(theta/2);
	z_in = axis[2] * sin(theta/2);
	w_in = cos(theta/2);
	this->set_xyzw(x_in, y_in, z_in, w_in);
}

#define mat float[3][3]
#define R00 R[0][0]
#define R01 R[0][1]
#define R02 R[0][2]
#define R03 R[0][3]
#define R10 R[1][0]
#define R11 R[1][1]
#define R12 R[1][2]
#define R13 R[1][3]
#define R20 R[2][0]
#define R21 R[2][1]
#define R22 R[2][2]
#define R23 R[2][3]
// rotation matrix constructor
quaternion::quaternion(float R[3][3]){
	float trace = R00 + R11 + R22;
	float S;
	if (trace > 0){
		S = std::sqrt(trace + 1.0) * 2;
		w = S / 4;
		x = (R21 - R12) / S;
		y = (R02 - R20) / S;
		z = (R10 - R01) / S;
	}
	else if ((R00 > R11) & (R00 > R22)){
		S = std::sqrt(1.0 + R00 - R11 - R22) * 2;
		w = (R21 - R12) / 2;
		x = S / 4;
		y = (R01 + R10) / S;
		z = (R02 + R20) / S;
	}
	else if (R11 > R22){
		S = std::sqrt(1 + R11 - R00 - R22) * 2;
		w = (R02 - R20) / 2;
		x = (R01 + R10) / S;
		y = S / 4;
		z = (R12 + R21) / S;
	}
	else{
		S = std::sqrt(1 + R22 - R00 - R11) * 2;
		w = (R10 - R01) / 2;
		x = (R02 + R20) / S;
		y = (R12 + R21) / S;
		z = S / 4;
	}
	this->normalize();
}

// euler angles constructor
quaternion::quaternion(float roll, float pitch, float yaw){
		float cos_roll = cos(0.5*roll);
		float cos_pitch = cos(0.5*pitch);
		float cos_yaw = cos(0.5*yaw);
		float sin_roll = sin(0.5*roll);
		float sin_pitch = sin(0.5*pitch);
		float sin_yaw = sin(0.5*yaw);
		w = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw;
		x = sin_roll * cos_pitch * cos_yaw + cos_roll * sin_pitch * sin_yaw;
		y = cos_roll * sin_pitch * cos_yaw - sin_roll * cos_pitch * sin_yaw;
		z = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw;
}

void quaternion::set_xyzw(float x_in, float y_in, float z_in, float w_in){
	x = x_in;
	y = y_in;
	z = z_in;
	w = w_in;
	this->normalize();
};

//* * * * * * * * * * * * * *
// GET REPRESENTATION METHODS
//* * * * * * * * * * * * * * 
std::vector<float> quaternion::get_xyzw(){
	std::vector<float> xyzw_vec;
	xyzw_vec.push_back(x);
	xyzw_vec.push_back(y);
	xyzw_vec.push_back(z);
	xyzw_vec.push_back(w);
	return xyzw_vec;
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

void quaternion::get_axis_angle(float &theta, float axis[]){
	// Something to check for the 0 case
	theta = acos(w) * 2;
	float vx, vy, vz;
	vx = x / sin(theta * 0.5);
	vy = y / sin(theta * 0.5);
	vz = z / sin(theta * 0.5);
	axis[0] = vx;
	axis[1] = vy;
	axis[2] = vz;
};

void quaternion::get_rotation_matrix(float R[3][3]){
		R[0][0] = w*w + x*x - y*y - z*z;
		R[1][1] = w*w - x*x + y*y - z*z;
		R[2][2] = w*w - x*x - y*y + z*z;
		R[1][0] = 2*x*y + 2*w*z;
		R[0][1] = 2*x*y - 2*w*z;
		R[2][0] = 2*x*z - 2*w*y;
		R[0][2] = 2*x*z + 2*w*y;
		R[2][1] = 2*y*z + 2*w*x;
		R[1][2] = 2*y*z - 2*w*x;
};

void quaternion::get_euler_angles(float rpy[3]){
	float roll, pitch, yaw;
	float sinr_cosp = 2 * (w * x + y * z);
	float cosr_cosp = 1 - 2 * (x*x + y*y);
	roll = atan2(sinr_cosp, cosr_cosp);
	float sinp = 2 * (w * y - z * x);
	if (abs(sinp) >=1){
		pitch = copysign(1.0, sinp) * pi()/2;
	}
	else{
		pitch = asin(sinp);
	}

	float siny_cosp = 2 * (w * z + x * y);
	float cosy_cosp = 1 - 2 * (y*y + z*z);
	yaw = atan2(siny_cosp, cosy_cosp);
	rpy[0] = roll;
	rpy[1] = pitch;
	rpy[2] = yaw;
};

//* * * * * * * * * * * * * *
// OPERATOR METHODS
//* * * * * * * * * * * * * * 
std::ostream& operator<< (std::ostream &out, quaternion const& q){
	char buffer [100];
	sprintf(buffer, "Quaternion <%.4f, %.4f, %.4f, %.4f>", q.x, q.y, q.z, q.w);
	out << buffer;
	return out;
}

quaternion operator+ (quaternion& q_self, quaternion& other){
	return q_self.relative_rotation_by(other);
}

quaternion operator- (quaternion& q_self, quaternion& other){
	quaternion conj = other.get_conjugate();
	return q_self.relative_rotation_by(conj);
}

//* * * * * * * * * * * * * *
// QUATERNION OPERATION METHODS
//* * * * * * * * * * * * * * 
quaternion quaternion::quat_mult(quaternion &q2){
	float w_out, x_out, y_out, z_out;
	w_out = this->get_w() * q2.get_w() - this->get_x() * q2.get_x() - this->get_y() * q2.get_y() - this->get_z() * q2.get_z();
	x_out = this->get_w() * q2.get_x() + this->get_x() * q2.get_w() + this->get_y() * q2.get_z() - this->get_z() * q2.get_y();
	y_out = this->get_w() * q2.get_y() - this->get_x() * q2.get_z() + this->get_y() * q2.get_w() + this->get_z() * q2.get_x();
	z_out = this->get_w() * q2.get_z() + this->get_x() * q2.get_y() - this->get_y() * q2.get_x() + this->get_z() * q2.get_w();
	return quaternion(x_out, y_out, z_out, w_out);
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

quaternion quaternion::global_rotation_by(quaternion &q_to_rotate){
	return this->quat_mult(q_to_rotate);
};

quaternion quaternion::relative_rotation_by(quaternion &q_rotate_by){
	return q_rotate_by.quat_mult(*this);
};

void quaternion::rotate_point(float pt[3], float pt_out[3]){
	quaternion p = quaternion(pt[0], pt[1], pt[2], 0.0);
	quaternion qp = this->quat_mult(p);
	quaternion q_conj = this->get_conjugate();
	quaternion qout = qp.quat_mult(q_conj);
	pt_out[0] = qout.get_x();
	pt_out[1] = qout.get_y();
	pt_out[2] = qout.get_z();
};

//* * * * * * * * * * * * * *
// PYTHON METHODS
//* * * * * * * * * * * * * * 
quaternion::quaternion(py::list q){
	float q0 = py::extract<float>(q[0]);
	float q1 = py::extract<float>(q[1]);
	float q2 = py::extract<float>(q[2]);
	float q3 = py::extract<float>(q[3]);
	this->set_xyzw(q0, q1, q2, q3);
};

py::list quaternion::py_get_xyzw(){
	py::list xyzw;
	xyzw.append(x);
	xyzw.append(y);
	xyzw.append(z);
	xyzw.append(w);
	return xyzw;
};


np::ndarray quaternion::py_np_get_xyzw(){
	py::list xyzw;
	xyzw.append(x);
	xyzw.append(y);
	xyzw.append(z);
	xyzw.append(w);
	np::ndarray np_list = np::array (xyzw);
	return np_list;
};

void quaternion::py_get_axis_angle(float &theta, py::list axis){
	// Something to check for the 0 case
	float axis_c[3];
	this->get_axis_angle(theta, axis_c);
	axis.append(axis_c[0]);
	axis.append(axis_c[1]);
	axis.append(axis_c[2]);
};

