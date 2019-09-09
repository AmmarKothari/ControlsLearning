import numpy as np



class quaternion(object):
	def __init__(self, q = None):
		if q is None:
			self.quat = np.array([0, 0, 0, 1])
		else:
			assert len(q) == 4
			q_norm = np.array(q) / np.linalg.norm(q)
			self.quat = q_norm

	@staticmethod
	def from_axis_angle(theta, axis):
		assert len(axis) == 3
		x = axis[0] * np.sin(theta/2)
		y = axis[1] * np.sin(theta/2)
		z = axis[2] * np.sin(theta/2)
		w = np.cos(theta/2)
		return quaternion([x, y, z, w])

	@staticmethod
	def from_rotation_matrix(R):
		assert R.shape == (3,3)
		trace = R[0, 0] + R[1,1] + R[2,2]
		if trace > 0:
			S = np.sqrt(trace + 1.0) * 2
			w = S / 4
			x = (R[2,1] - R[1,2]) / S
			y = (R[0,2] - R[2,0]) / S
			z = (R[1,0] - R[0,1]) / S
		elif ((R[0,0] > R[1,1]) & (R[0,0] > R[2,2])):
			S = np.sqrt(1.0 + R[0,0] - R[1,1] - R[2,2]) * 2
			w = (R[2,1] - R[1,2]) / 2
			x = S / 4
			y = (R[0,1] + R[1,0]) / S
			z = (R[0,2] + R[2,0]) / S
		elif (R[1,1] > R[2,2]):
			S = np.sqrt(1 + R[1,1] - R[0,0] - R[2,2]) * 2
			w = (R[0,2] - R[2,0]) / 2
			x = (R[0,1] + R[1,0]) / S
			y = S / 4
			z = (R[1,2] + R[2,1]) / S
		else:
			S = np.sqrt(1 + R[2,2] - R[0,0] - R[1,1]) * 2
			w = (R[1,0] - R[0,1]) / 2
			x = (R[0,2] + R[2,0]) / S
			y = (R[1,2] + R[2,1]) / S
			z = S / 4
		return quaternion([x, y, z, w])

	@staticmethod
	def from_euler_angles(roll, pitch, yaw):
		cos_roll = np.cos(0.5*roll)
		cos_pitch = np.cos(0.5*pitch)
		cos_yaw = np.cos(0.5*yaw)
		sin_roll = np.sin(0.5*roll)
		sin_pitch = np.sin(0.5*pitch)
		sin_yaw = np.sin(0.5*yaw)
		w = cos_roll * cos_pitch * cos_yaw + sin_roll * sin_pitch * sin_yaw
		x = sin_roll * cos_pitch * cos_yaw + cos_roll * sin_pitch * sin_yaw
		y = cos_roll * sin_pitch * cos_yaw - sin_roll * cos_pitch * sin_yaw
		z = cos_roll * cos_pitch * sin_yaw - sin_roll * sin_pitch * cos_yaw
		return quaternion([x, y, z, w])
		
	@staticmethod
	def quat_mult(q1, q2):
		w = q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
		x = q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y
		y = q1.w * q2.y - q1.x * q2.z + q1.y * q2.w + q1.z * q2.x
		z = q1.w * q2.z + q1.x * q2.y - q1.y * q2.x + q1.z * q2.w
		return quaternion([x, y, z, w])

	def get_x(self):
		return self.quat[0]

	def get_y(self):
		return self.quat[1]

	def get_z(self):
		return self.quat[2]

	def get_w(self):
		return self.quat[3]

	def get_xyzw(self):
		return self.quat

	@property
	def x(self):
		return self.quat[0]

	@property
	def y(self):
		return self.quat[1]

	@property
	def z(self):
		return self.quat[2]

	@property
	def w(self):
		return self.quat[3]

	@property
	def xyzw(self):
		return self.quat

	def get_conjugate(self):
		return quaternion(self.quat * [-1, -1, -1, 1])

	def get_axis_angle(self):
		# unit quaternion means is no rotation around z but could be anything
		# there is ambiguity in these situations
		if np.isclose(self.quat[:3], [0, 0, 0]).all and np.isclose(self.quat[3], 1):
			return 0, np.array([0, 0, 1])
		theta = np.arccos(self.quat[3]) / 0.5
		vx, vy, vz = self.quat[:-1]/ np.sin(theta*0.5)
		return theta, np.array([vx, vy, vz])

	def get_rotation_matrix(self):
		T = np.eye(3)
		T[0,0] = self.w**2 + self.x**2 - self.y**2 - self.z**2
		T[1,1] = self.w**2 - self.x**2 + self.y**2 - self.z**2
		T[2,2] = self.w**2 - self.x**2 - self.y**2 + self.z**2
		T[1,0] = 2*self.x*self.y + 2*self.w*self.z
		T[0,1] = 2*self.x*self.y - 2*self.w*self.z
		T[2,0] = 2*self.x*self.z - 2*self.w*self.y
		T[0,2] = 2*self.x*self.z + 2*self.w*self.y
		T[2,1] = 2*self.y*self.z + 2*self.w*self.x
		T[1,2] = 2*self.y*self.z - 2*self.w*self.x
		return T

	def get_euler_angles(self):
		sinr_cosp = 2 * (self.w * self.x + self.y * self.z)
		cosr_cosp = 1 - 2 * (self.x**2 + self.y**2)
		roll = np.arctan2(sinr_cosp, cosr_cosp)
		sinp = 2 * (self.w * self.y - self.z * self.x)
		if abs(sinp) >=1:
			pitch = sign(sinp) * np.pi/2
		else:
			pitch = np.arcsin(sinp)

		siny_cosp = 2 * (self.w * self.z + self.x * self.y)
		cosy_cosp = 1 - 2 * (self.y**2 + self.z**2)
		yaw = np.arctan2(siny_cosp, cosy_cosp)
		return [roll, pitch, yaw]

	def global_rotation_by(self, q_to_rotate):
		return self.quat_mult(self, q_to_rotate)

	def relative_rotation_by(self, q_rotate_by):
		return self.quat_mult(q_rotate_by, self)

	def rotate_point(self, vec):
		assert len(vec) >= 3
		if len(vec) == 3:
			vec = vec[:] + [0]
		pin = quaternion(vec)
		pout = quaternion.quat_mult(quaternion.quat_mult(self, pin), self.get_conjugate())
		return pout.get_xyzw()[:3]

	def __repr__(self):
		return '[{:.3f}, {:.3f}, {:.3f}, {:.3f}]'.format(*self.quat)

	def __str__(self):
		return self.__repr__()

	def __eq__(self, other):
		if isinstance(other, self.__class__):
			return all([np.isclose(o, i) for o,i in zip(self.quat, other.quat)])
		else:
			return False

	def __add__(self, other):
		return self.relative_rotation_by(other)

	def __sub__(self, other):
		return self.relative_rotation_by(other.get_conjugate())

	def __mul__(self, other):
		return self + other

	def __truediv__(self, other):
		return self - other





