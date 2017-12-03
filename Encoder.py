import numpy as np
import matplotlib.pyplot as plt

class Encoder(object): # class to simulate an encoder
	def __init__(self, conv):
		self.conv = float(conv)
		self.q_prev = 0
		self.q_cur = 0
		self.enc_cur = 0
		self.bits = 1024
		self.first_update = True

	def update(self, q_cur): #update encoder state based on simulated dynamics
		if self.first_update:
			self.q_prev = q_cur
			self.first_update = False
		if np.isnan(q_cur - self.q_prev):
			pdb.set_trace()
		delta_enc = int((q_cur - self.q_prev) / self.conv) # converted value from encoder
		# print("Q_cur: %s" %q_cur)
		if abs(delta_enc) >= 1:
			self.q_prev = self.q_cur
		self.q_cur = q_cur
		self.enc_cur += delta_enc
		# simulate wrap around
		if self.enc_cur > self.bits:
			self.enc_cur -= self.bits * 2
		elif self.enc_cur < -self.bits:
			self.enc_cur =+ self.bits * 2
		return self.enc_cur

	def test(self):
		points = 1000
		q_new = np.zeros(1000)
		for i,q in enumerate(np.linspace(0,20*np.pi, 1000)):
			# pdb.set_trace()
			q_new[i] = E.update(q)
		plt.plot(q_new, 'ro')
		plt.show()