import quaternion_py
import unittest
import numpy as np
import quaternion_wrap
import pdb
import functools

# def class_decorator(cls):
# 	def wrapper(func_list, *args):
# 		print(args[0])
# 		for setup in func_list:
# 			setup()
# 			func(args[0])

# 	for name, method in cls.__dict__.iteritems():
# 		if hasattr(method, "test_to_run"):
# 			if method.test_to_run == "both":
# 				print('Running both tests')
# 				test_to_run = (args[0]._quat_py_setUp, args[0]._quat_wrap_setUp)
# 			if method.test_to_run == "py":
# 				print('Running py tests')
# 				test_to_run = (args[0]._quat_py_setUp, )
# 			if method.test_to_run == "cpp":
# 				print('Running cpp tests')
# 				test_to_run = (args[0]._quat_wrap_setUp, )
# 			return functools.partial(wrapper, test_to_run)
	# return cls

# def run_both(view):
# 	view.test_to_run = 'both'
# 	return view

# def run_py(view):
# 	view.test_to_run = 'py'

# def run_cpp(view):
# 	view.test_to_run = 'cpp'


def run_both(func):
	def wrapper(*args):
		for setup in (args[0]._quat_py_setUp, args[0]._quat_wrap_setUp):
			setup()
			func(*args)
	return wrapper

def run_py(func):
	def wrapper(*args):
		for setup in (args[0]._quat_py_setUp, ):
			setup()
			func(*args)
	return wrapper

def run_cpp(func):
	def wrapper(*args):
		for setup in (args[0]._quat_wrap_setUp, ):
			setup()
			func(*args)
	return wrapper

# @class_decorator
class QuaternionPythonTest(unittest.TestCase):

	def _quat_py_setUp(self):
		sq1_2 = np.sqrt(1./2)
		self.q_unit = quaternion_py.quaternion()
		self.q_x = quaternion_py.quaternion([1.0, 0, 0, 0])
		self.q_x_2 = quaternion_py.quaternion([sq1_2, 0, 0, sq1_2])
		self.q_y = quaternion_py.quaternion([0, 1.0, 0, 0])
		self.q_y_2 = quaternion_py.quaternion([0, sq1_2, 0, sq1_2])
		self.q_z = quaternion_py.quaternion([0, 0, 1.0, 0])
		self.q_z_2 = quaternion_py.quaternion([0, 0, sq1_2, sq1_2])
		self.q_norm = quaternion_py.quaternion([100, 0, 0, 0])

	def _quat_wrap_setUp(self):
		sq1_2 = np.sqrt(1./2)
		self.q_unit = quaternion_wrap.quaternion()
		self.q_x = quaternion_wrap.quaternion([1.0, 0, 0, 0])
		self.q_x_2 = quaternion_wrap.quaternion([sq1_2, 0, 0, sq1_2])
		self.q_y = quaternion_wrap.quaternion([0, 1.0, 0, 0])
		self.q_y_2 = quaternion_wrap.quaternion([0, sq1_2, 0, sq1_2])
		self.q_z = quaternion_wrap.quaternion([0, 0, 1.0, 0])
		self.q_z_2 = quaternion_wrap.quaternion([0, 0, sq1_2, sq1_2])
		self.q_norm = quaternion_wrap.quaternion([100, 0, 0, 0])

	@run_both
	def test_empty_constructor(self):
		self.assertEqual(self.q_unit.get_x(), 0.0)
		self.assertEqual(self.q_unit.get_y(), 0.0)
		self.assertEqual(self.q_unit.get_z(), 0.0)
		self.assertEqual(self.q_unit.get_w(), 01.0)

	@run_both
	def test_value_constructor(self):
		self.assertEqual(self.q_x.get_x(), 1.0)
		self.assertEqual(self.q_x.get_y(), 0.0)
		self.assertEqual(self.q_x.get_z(), 0.0)
		self.assertEqual(self.q_x.get_w(), 0.0)
	
	@run_py
	def test_axis_angle_constructor(self):
		q_unit = quaternion_py.quaternion.from_axis_angle(0, [0, 0, 1])
		self.assertEqual(q_unit, self.q_unit)
		q_x = quaternion_py.quaternion.from_axis_angle(np.pi, [1, 0, 0])
		self.assertEqual(q_x, self.q_x)

	@run_py
	def test_rotation_matrix_constructor(self):
		R_unit = np.eye(3)
		q = quaternion_py.quaternion.from_rotation_matrix(R_unit)
		self.assertEqual(q.get_xyzw().tolist(), [0, 0, 0, 1])

		R_x = np.array([[ 1.,  0.,  0.],
				        [ 0.,  -1., 0.],
				        [ 0.,  0.,  -1.]])
		qx = quaternion_py.quaternion.from_rotation_matrix(R_x)
		self.assertEqual(qx.get_xyzw().tolist(), [1, 0, 0, 0])

		R_half_x = np.array([[ 1.,  0.,  0.],
					         [ 0.,  0., -1.],
					         [ 0.,  1.,  0.]])
		qx_half = quaternion_py.quaternion.from_rotation_matrix(R_half_x)
		np.testing.assert_almost_equal(qx_half.get_xyzw().tolist(), [.707, 0, 0, .707], 3)

		R_y = np.eye(3)
		R_y[0, 0] = -1
		R_y[2, 2] = -1
		qy = quaternion_py.quaternion.from_rotation_matrix(R_y)
		self.assertEqual(qy.get_xyzw().tolist(), [0, 1, 0, 0])

		R_half_y = np.array([[ 0.,  0.,  1.],
						       [ 0.,  1.,  0.],
						       [-1.,  0.,  0.]])
		qy_half = quaternion_py.quaternion.from_rotation_matrix(R_half_y)
		np.testing.assert_almost_equal(qy_half.get_xyzw().tolist(), [0, 0.707, 0, .707], 3)

		R_z = np.eye(3)
		R_z[0, 0] = -1
		R_z[1, 1] = -1
		qz = quaternion_py.quaternion.from_rotation_matrix(R_z)
		self.assertEqual(qz.get_xyzw().tolist(), [0, 0, 1, 0])

	@run_py
	def test_euler_angles_constructor(self):
		q_unit = quaternion_py.quaternion.from_euler_angles(0, 0, 0)
		self.assertEqual(q_unit, self.q_unit)
		q_x = quaternion_py.quaternion.from_euler_angles(np.pi, 0, 0)
		self.assertEqual(q_x, self.q_x)
		q_y = quaternion_py.quaternion.from_euler_angles(0, np.pi, 0)
		self.assertEqual(q_y, self.q_y)
		q_z = quaternion_py.quaternion.from_euler_angles(0, 0, np.pi)
		self.assertEqual(q_z, self.q_z)

	@run_both
	def test_normalize(self):
		self.assertEqual(self.q_norm.get_xyzw().tolist(), [1.0, 0.0, 0.0, 0.0])

	@run_py
	def test_axis_angle(self):
		theta, axis = self.q_unit.get_axis_angle()
		self.assertEqual(theta, 0)
		self.assertEqual(axis.tolist(), [0, 0, 1])

		theta, axis = self.q_x.get_axis_angle()
		self.assertEqual(theta, np.pi)
		self.assertAlmostEqual(axis.tolist(), [1, 0, 0])
		theta, axis = self.q_y.get_axis_angle()
		self.assertEqual(theta, np.pi)
		self.assertAlmostEqual(axis.tolist(), [0, 1, 0])
		theta, axis = self.q_z.get_axis_angle()
		self.assertEqual(theta, np.pi)
		self.assertAlmostEqual(axis.tolist(), [0, 0, 1])

	@run_py
	def test_rotation_matrix(self):
		self.assertEqual(np.eye(3).tolist(), self.q_unit.get_rotation_matrix().tolist())
		R_x = np.array([[ 1.,  0.,  0.],
				        [ 0.,  -1., 0.],
				        [ 0.,  0.,  -1.]])
		self.assertEqual(self.q_x.get_rotation_matrix().tolist(), R_x.tolist())

	@run_both
	def test_conjugate(self):
		self.assertEqual(self.q_unit.get_conjugate().get_xyzw().tolist(), [0, 0, 0, 1])
		self.assertEqual(self.q_x.get_conjugate().get_xyzw().tolist(), [-1, 0, 0, 0])

	@run_py
	def test_global_rotation(self):
		q_x = self.q_unit.global_rotation_by(self.q_x)
		self.assertEqual(q_x, self.q_x)

		q_out = self.q_x.global_rotation_by(self.q_z)
		self.assertEqual(q_out.get_xyzw().tolist(), [0.0, -1.0, 0.0, 0.0])

		q_out = self.q_x_2.global_rotation_by(self.q_z_2)
		self.assertEqual(q_out.get_xyzw().tolist(), [0.500, -0.500, 0.500, 0.500])		

	@run_py
	def test_relative_rotation(self):
		q_x = self.q_x.relative_rotation_by(self.q_unit)
		self.assertEqual(q_x, self.q_x)

		q_out = self.q_x.relative_rotation_by(self.q_z)
		self.assertEqual(q_out.get_xyzw().tolist(), [0.0, 1.0, 0.0, 0.0])

		q_out = self.q_x_2.relative_rotation_by(self.q_z_2)
		self.assertEqual(q_out.get_xyzw().tolist(), [0.500, 0.500, 0.500, 0.500])

	@run_py
	def test_point_rotation(self):
		p_in = [1, 0, 0]
		self.assertEqual(self.q_x.rotate_point(p_in).tolist(), [1, 0, 0])
		self.assertEqual(self.q_y.rotate_point(p_in).tolist(), [-1, 0, 0])
		self.assertEqual(self.q_z.rotate_point(p_in).tolist(), [-1, 0, 0])

	@run_py
	def test_repr(self):
		repr = '[0.000, 0.000, 0.000, 1.000]'
		self.assertEqual(self.q_unit.__repr__(), repr)

	@run_py
	def test_equality(self):
		self.assertEqual(self.q_unit, self.q_unit)
		self.assertEqual(self.q_x, self.q_x)
		self.assertEqual(self.q_y, self.q_y)
		self.assertEqual(self.q_z, self.q_z)

	@run_py
	def test_add(self):
		self.assertEqual(self.q_x, self.q_unit + self.q_x)

	@run_py
	def test_sub(self):
		self.assertEqual(self.q_unit, self.q_x - self.q_x)

	@run_both
	def test_set_xyzw(self):
		# import pdb
		# pdb.set_trace()
		self.q_unit.set_xyzw(1, 0, 0, 0)
		self.assertEqual(self.q_unit, self.q_x)


	if __name__ == '__main__':
		unittest.main()
