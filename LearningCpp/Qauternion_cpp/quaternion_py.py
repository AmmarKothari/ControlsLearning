import numpy as np



class quaternion(object):
	def __init__(self, q = None):
		if q is None:
			self.quat = [0, 0, 0, 1]
		else:
			assert len(q) == 4
			q_norm = q / np.linalg.norm(q)
			self.quat = q




