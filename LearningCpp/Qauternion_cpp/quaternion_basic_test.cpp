// A basic test for quaternion operations

#define CATCH_CONFIG_RUNNER
#include <catch2/catch.hpp>
#include "quaternion.h"
#include <iostream>
#include <vector>
#include <boost/python.hpp>
#include <boost/math/constants/constants.hpp>

// bool compare_xyzw(quaternion q1, float v[4]){
// 	std::vector<float> xyzw;
// 	xyzw = q1.get_xyzw();
// 	for (int i=0; i<4; i++){
// 		Catch::REQURE( xyzw[i] == v[i]);
// 	};
// }

TEST_CASE( "Quaternion initialization", "[quat_init]" ) {
	quaternion q1;
	// float v[4] = {0.0, 0.0, 0.0, 1.0};
	// compare_xyzw(q1, v);
    REQUIRE( q1.get_x() == 0.0 );
    REQUIRE( q1.get_y() == 0.0 );
    REQUIRE( q1.get_z() == 0.0 );
    REQUIRE( q1.get_w() == 1.0 );

	quaternion q2(0.0, 0.0, 1.0, 0.0);
    REQUIRE( q2.get_x() == 0.0 );
    REQUIRE( q2.get_y() == 0.0 );
    REQUIRE( q2.get_z() == 1.0 );
    REQUIRE( q2.get_w() == 0.0 );

	float q [4] = {1.0, 0.0, 0.0, 0.0};
	quaternion q4(q);
    REQUIRE( q4.get_x() == 1.0 );
    REQUIRE( q4.get_y() == 0.0 );
    REQUIRE( q4.get_z() == 0.0 );
    REQUIRE( q4.get_w() == 0.0 );

    float axis[3] = {0.0, 0.0, 1.0};
    float pi;
    pi = boost::math::constants::pi<float>();
    float theta = pi/2;
    quaternion quat_axis_angle(theta, axis);
	Approx target_pi_2 = Approx(0.707).epsilon(0.01);
    REQUIRE( quat_axis_angle.get_x() == 0.0 );
    REQUIRE( quat_axis_angle.get_y() == 0.0 );
    REQUIRE( quat_axis_angle.get_z() == target_pi_2 );
    REQUIRE( quat_axis_angle.get_w() == target_pi_2 );

    float R_unit[3][3] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};
    quaternion quat_rot_mat(R_unit);
    REQUIRE( quat_rot_mat.get_x() == 0.0);
    REQUIRE( quat_rot_mat.get_y() == 0.0);
    REQUIRE( quat_rot_mat.get_z() == 0.0);
    REQUIRE( quat_rot_mat.get_w() == 1.0);

    float R_z[3][3] = {{-1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}};
    quaternion quat_rot_mat_z(R_z);
    REQUIRE( quat_rot_mat_z.get_x() == 0.0);
    REQUIRE( quat_rot_mat_z.get_y() == 0.0);
    REQUIRE( quat_rot_mat_z.get_z() == 1.0);
    REQUIRE( quat_rot_mat_z.get_w() == 0.0);

    float rpy_unit[3] = {0.0, 0.0, 0.0};
    quaternion quat_rpy_unit(rpy_unit[0], rpy_unit[1], rpy_unit[2]);
    REQUIRE( quat_rpy_unit.get_x() == 0.0);
    REQUIRE( quat_rpy_unit.get_y() == 0.0);
    REQUIRE( quat_rpy_unit.get_z() == 0.0);
    REQUIRE( quat_rpy_unit.get_w() == 1.0);

}

TEST_CASE("Quaternion values set", "[quat_set]"){
	quaternion q1;
	q1.set_xyzw(0.0, 0.0, 1.0, 0.0);
    REQUIRE( q1.get_x() == 0.0 );
    REQUIRE( q1.get_y() == 0.0 );
    REQUIRE( q1.get_z() == 1.0 );
    REQUIRE( q1.get_w() == 0.0 );
}

//* * * * * * * * * * * * * *
// GET REPRESENTATION METHODS
//* * * * * * * * * * * * * * 
TEST_CASE("Quaternion get values", "[quat_get]"){
	quaternion q1(1.0, 0.0, 0.0, 0.0);
	std::vector<float> xyzw = q1.get_xyzw();
    REQUIRE( xyzw[0] == 1.0 );
    REQUIRE( xyzw[1] == 0.0 );
    REQUIRE( xyzw[2] == 0.0 );
    REQUIRE( xyzw[3] == 0.0 );
}

TEST_CASE("Quaternion axis angle", "[quat_axis_angle]"){
	quaternion q1(0.0, 0.0, 1.0, 0.0);
	float theta;
	float axis[3];
	q1.get_axis_angle(theta, axis);
	Approx target_theta = Approx(3.14).epsilon(0.01);
    REQUIRE( theta == target_theta );
    REQUIRE( axis[0] == 0.0 );
    REQUIRE( axis[1] == 0.0 );
    REQUIRE( axis[2] == 1.0 );
}

TEST_CASE("Quaternion rotation matrix", "[quat_rot_mat]"){
	quaternion q1(0.0, 0.0, 0.0, 1.0);
	float R[3][3];
	q1.get_rotation_matrix(R);
	REQUIRE( R[0][0] == 1.0);
	REQUIRE( R[1][1] == 1.0);
	REQUIRE( R[2][2] == 1.0);
	REQUIRE( R[0][1] == 0.0);
	REQUIRE( R[1][0] == 0.0);
}

TEST_CASE("Quaternion euler angles", "[quat_euler_angles]"){
	quaternion q1(0.0, 0.0, 0.0, 1.0);
	float rpy[3];
	q1.get_euler_angles(rpy);
	REQUIRE( rpy[0] == 0.0);
	REQUIRE( rpy[1] == 0.0);
	REQUIRE( rpy[2] == 0.0);
}


//* * * * * * * * * * * * * *
// QUATERNION OPERATION METHODS
//* * * * * * * * * * * * * * 
TEST_CASE("Quaternion multiply", "[quat_mult]"){
	quaternion q_unit(0.0, 0.0, 0.0, 1.0);
	quaternion q1(0.0, 0.0, 1.0, 0.0);
	quaternion q_out;
	q_out = q1.quat_mult(q_unit);
    REQUIRE( q_out.get_x() == 0.0 );
    REQUIRE( q_out.get_y() == 0.0 );
    REQUIRE( q_out.get_z() == 1.0 );
    REQUIRE( q_out.get_w() == 0.0 );
}

TEST_CASE("Quaternion normalize", "[quat_norm]"){
	quaternion q1(1.0, 1.0, 1.0, 1.0);
    REQUIRE( q1.get_x() == 0.5 );
    REQUIRE( q1.get_y() == 0.5 );
    REQUIRE( q1.get_z() == 0.5 );
    REQUIRE( q1.get_w() == 0.5 );

    quaternion q2(100.0, 0.0, 0.0, 0.0);
    REQUIRE( q2.get_x() == 1.0 );
    REQUIRE( q2.get_y() == 0.0 );
    REQUIRE( q2.get_z() == 0.0 );
    REQUIRE( q2.get_w() == 0.0 );
}

TEST_CASE("Quaternion conjugate", "[quat_conj]"){
	quaternion q1(1.0, 0.0, 0.0, 0.0);
	quaternion q1_conj = q1.get_conjugate();
    REQUIRE( q1_conj.get_x() == -1.0 );
    REQUIRE( q1_conj.get_y() == 0.0 );
    REQUIRE( q1_conj.get_z() == 0.0 );
    REQUIRE( q1_conj.get_w() == 0.0 );
}

TEST_CASE("Quaternion global rotation by", "[quat_global_rot]"){
	quaternion q1(1.0, 0.0, 0.0, 0.0);
	quaternion q2(0.0, 0.0, 1.0, 0.0);
	quaternion q_out = q1.global_rotation_by(q2);
    REQUIRE( q_out.get_x() == 0.0 );
    REQUIRE( q_out.get_y() == -1.0 );
    REQUIRE( q_out.get_z() == 0.0 );
    REQUIRE( q_out.get_w() == 0.0 );
}

TEST_CASE("Quaternion relative rotation by", "[quat_relative_rot]"){
	quaternion qx(1.0, 0.0, 0.0, 0.0);
	quaternion qz(0.0, 0.0, 1.0, 0.0);
	quaternion q_out = qx.relative_rotation_by(qz);
    REQUIRE( q_out.get_x() == 0.0 );
    REQUIRE( q_out.get_y() == 1.0 );
    REQUIRE( q_out.get_z() == 0.0 );
    REQUIRE( q_out.get_w() == 0.0 );
}

TEST_CASE("Quaternion rotate point", "[quat_rot_point]"){
	float pt[3] = {1, 0, 0};
	float p_out[3];
	quaternion qx(1.0, 0.0, 0.0, 0.0);
	quaternion qy(0.0, 1.0, 0.0, 0.0);
	quaternion qz(0.0, 0.0, 1.0, 0.0);
	qx.rotate_point(pt, p_out);
	float px_out[3] = {1, 0, 0};
    REQUIRE( p_out[0] == px_out[0] );
    REQUIRE( p_out[1] == px_out[1] );
    REQUIRE( p_out[2] == px_out[2] );
    qy.rotate_point(pt, p_out);
	float py_out[3] = {-1, 0, 0};
    REQUIRE( p_out[0] == py_out[0] );
    REQUIRE( p_out[1] == py_out[1] );
    REQUIRE( p_out[2] == py_out[2] );
    qz.rotate_point(pt, p_out);
	float pz_out[3] = {-1, 0, 0};
    REQUIRE( p_out[0] == pz_out[0] );
    REQUIRE( p_out[1] == pz_out[1] );
    REQUIRE( p_out[2] == pz_out[2] );
}

//* * * * * * * * * * * * * *
// OPERATOR METHODS
//* * * * * * * * * * * * * *
TEST_CASE("Add operator", "[op_add]"){
	quaternion qx(1.0, 0.0, 0.0, 0.0);
	quaternion q_out = qx + qx;
    REQUIRE( q_out.get_x() == 0.0 );
    REQUIRE( q_out.get_y() == 0.0 );
    REQUIRE( q_out.get_z() == 0.0 );
    REQUIRE( q_out.get_w() == -1.0 );

}

TEST_CASE("Sub operator", "[op_sub]"){
	quaternion qx(1.0, 0.0, 0.0, 0.0);
	quaternion q_out = qx - qx;
    REQUIRE( q_out.get_x() == 0.0 );
    REQUIRE( q_out.get_y() == 0.0 );
    REQUIRE( q_out.get_z() == 0.0 );
    REQUIRE( q_out.get_w() == 1.0 );

}

TEST_CASE("<< operator", "[op_<<]"){
	quaternion qx(1.0, 0.0, 0.0, 0.0);
	std::cout << qx << std::endl;
}

//* * * * * * * * * * * * * *
// PYTHON METHODS
//* * * * * * * * * * * * * * 

TEST_CASE("Quaternion python xyzw", "[quat_py_xyzw]") {
	quaternion q1(0.0, 0.0, 1.0, 0.0);
	py::list out_list;
	out_list = q1.py_get_xyzw();
    REQUIRE( out_list[0] == 0.0 );
    REQUIRE( out_list[1] == 0.0 );
    REQUIRE( out_list[2] == 1.0 );
    REQUIRE( out_list[3] == 0.0 );
}

TEST_CASE("Quaternion python get axis angle", "[quat_py_get_axis_angle]") {
    float pi;
    pi = boost::math::constants::pi<float>();
	Approx target = Approx(pi).epsilon(0.01);

	py::list axis;
	float theta;
	quaternion q1(0.0, 0.0, 1.0, 0.0);
	q1.py_get_axis_angle(theta, axis);
    REQUIRE( axis[0] == 0.0 );
    REQUIRE( axis[1] == 0.0 );
    REQUIRE( axis[2] == 1.0 );
    REQUIRE( theta ==  target);

    py::list axis_2;
	float theta_2;
	quaternion q2(1.0, 0.0, 0.0, 0.0);
	q2.py_get_axis_angle(theta_2, axis_2);
    REQUIRE( axis_2[0] == 1.0 );
    REQUIRE( axis_2[1] == 0.0 );
    REQUIRE( axis_2[2] == 0.0 );
    REQUIRE( theta ==  target);
}

int main( int argc, char* argv[] ) {
  Py_Initialize();
  int result = Catch::Session().run( argc, argv );

  return result;
}

