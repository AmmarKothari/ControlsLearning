import quaternion_py
import unittest

# @unittest.skip("skipping abstract class")
# class AbstractQuaternion(unittest.TestCase):
	# def __init__(self, quat_test_class):
	# 	self.quat_class = quat_class




	def test_empty_constructor(self):
		self.assertEqual(self.q_unit.get_x(), 0.0)
		self.assertEqual(self.q_unit.get_y(), 0.0)
		self.assertEqual(self.q_unit.get_z(), 0.0)
		self.assertEqual(self.q_unit.get_w(), 01.0)

	def test_value_constructor(self):
		self.assertEqual(self.q_x.get_x(), 1.0)
		self.assertEqual(self.q_x.get_y(), 0.0)
		self.assertEqual(self.q_x.get_z(), 0.0)
		self.assertEqual(self.q_x.get_w(), 0.0)

	def test_normalize(self):
		self.assertEqual(self.q_norm.get_x(), 1.0)
		self.assertEqual(self.q_norm.get_y(), 0.0)
		self.assertEqual(self.q_norm.get_z(), 0.0)
		self.assertEqual(self.q_norm.get_w(), 0.0)

# @unittest.skip("skipping cpp quaternion")
class QuaternionAmmarExtTest(AbstractQuaternion):
	def setUp(self):
		from quaternion_ammar_ext import quaternion
		self.q_unit = quaternion()
		self.q_x = quaternion(1.0, 0, 0, 0)
		self.q_norm = quaternion(100, 0, 0, 0)

	def test_get(self):
		q = quaternion(1.0, 0, 0, 0)
		self.assertEqual(q.get_xyzw(), [1, 0, 0, 0])

class QuaternionPythonTest(AbstractQuaternion):
	def setUp(self):
		self.q_unit = quaternion_py.quaternion()
		self.q_x = quaternion_py.quaternion(1.0, 0, 0, 0)
		self.q_norm = quaternion_py.quaternion(100, 0, 0, 0)

	# def test_isupper(self):
	#     self.assertTrue('FOO'.isupper())
	#     self.assertFalse('Foo'.isupper())

	# def test_split(self):
	#     s = 'hello world'
	#     self.assertEqual(s.split(), ['hello', 'world'])
	#     # check that s.split fails when the separator is not a string
	#     with self.assertRaises(TypeError):
	#         s.split(2)


# if __name__ == '__main__':
# 	unittest.main()