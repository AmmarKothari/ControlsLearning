import control
import matplotlib.pyplot as plt
import numpy as np
from GeneralController import GeneralController

class PIDController(GeneralController):
	def __init__(self, t, P = 1.0, I = 100, D = 0):
		super(PIDController, self).__init__(t)
		self.P = P
		self.I = I
		self.D = D
		self.characteristicEquation()

	def setQTarget(self, q):
		super(PIDController, self).setQTarget(q)

	def characteristicEquation(self):
		P_CE = control.tf([self.P], [1])
		I_CE = control.tf([self.I], [1,0])
		D_CE = control.tf([self.D, 0], [1])
		self.CE = P_CE + I_CE + D_CE #parallel connection
		return self.CE

	def sysOpenCE(self, plantCE): # assumes controller is directly in front of plant
		return super(PIDController, self).sysOpenCE(plantCE)

	def sysClosedCE(self): # assumes that controller is before plant on feedforward step
		return super(PIDController, self).sysOpenCE(plantCE)

	def sysRecomp(self, P = None, I = None, D = None):
		if P is not None:
			self.P = P
		if I is not None:
			self.I = I
		if D is not None:
			self.D = D
		self.characteristicEquation()
		self.sysOpenCE(self.plantCE)
		self.sysClosedCE()
		print self.sysCCE

	def plotImpulseResponse(self, CE, axx = None, Plot=True):
		t,q = super(PIDController, self).plotImpulseResponse(CE, axx, Plot)
		return t,q

	def plotStepResponse(self, CE, axx = None, Plot=True):
		t, q = control.step_response(CE)
		if axx is not None:
			axx.plot(t,q, marker = np.random.choice(self.marker), label = 'P:%s, I:%s, D:%s' %(self.P, self.I, self.D))
			axx.legend()
		if Plot:
			plt.plot(t,q)
			plt.show()
		return t,q

	def update(self, q_cur): 
		self.e = self.q_target[0] - q_cur[0]
		if self.first_update:
			self.q_last = q_cur
			self.e_prev = self.e
			self.first_update = False
		self.e_total += self.e  # don't need dt, can roll it into gains
		self.delta_e = self.e - self.e_prev
		self.u_P = self.P * self.e # proportional
		self.u_I = self.I * self.e_total # integral
		self.u_D = self.D * (self.e - self.e_prev) # derivative, assuming time change
		self.u = self.u_P + self.u_D + self.u_I
		self.q_last = q_cur
		# pdb.set_trace()
		return self.u, self.e

	def plotRootLocus(self, rl_out, axx, linetype = '-'): #plots the output of a root_locus
		poles = rl_out[0]
		reals = np.real(poles)
		imags = np.imag(poles)
		real = [];
		imag = []
		for i in range(len(reals[0])):
			imag.append(imags[:,i])
			real.append(reals[:,i])
		# pdb.set_trace()
		plt.plot(np.array(real).flatten(), np.array(imag).flatten(), linetype, label = 'P:%s, I:%s, D:%s' %(self.P, self.I, self.D))
		plt.legend()