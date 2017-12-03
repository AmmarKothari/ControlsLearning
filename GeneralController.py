import control
import matplotlib.pyplot as plt
import numpy as np

class GeneralController(object):
	#meant to be a general control object
	#used when wanting to build your own CE to pass as the controller
	def __init__(self, t):
		self.dt = t # this is usually 1/10 as fast as the simulation
		self.last_t = 0
		self.e_total = 0
		self.e_prev = 0 
		self.first_update = True
		self.marker = ['+', '.', 'o', '*']
		self.CE = None

	def setQTarget(self, q):
		self.q_target = q
		self.q_last = np.zeros_like(q)

	def setCE(self, CE):
		self.CE = CE

	def characteristicEquation(self):
		#needs to be overloaded!
		return None

	def sysOpenCE(self, plantCE): # assumes controller is directly in front of plant
		if self.CE:
			self.plantCE = plantCE
			self.sysOCE = self.CE * plantCE #series connection
			return self.sysOCE
		else:
			print('Controller Not Set!')
			return None

	def sysClosedCE(self): # assumes that controller is before plant on feedforward step
		self.sysCCE = control.feedback(self.sysOCE, 1, sign = -1)
		return self.sysCCE

	def plotImpulseResponse(self, CE, axx = None, Plot=True):
		t, q = control.impulse_response(CE)
		if axx is not None:
			axx.plot(t,q, marker = np.random.choice(self.marker), label = 'P:%s, I:%s, D:%s' %(self.P, self.I, self.D))
			axx.legend()
		if Plot:
			plt.plot(t,q, marker = np.random.choice(self.marker), label = 'P:%s, I:%s, D:%s' %(self.P, self.I, self.D))
			plt.show()
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
		#needs to be overloaded!
		return None

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