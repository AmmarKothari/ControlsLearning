#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import pdb
from scipy.integrate import odeint
import control
import os
import pdb
import copy


cur_dir = os.getcwd()
os.sys.path.append(cur_dir + '/../ControlsGeneral/')
from Encoder import Encoder
from Motor import Motor
from PIDController import PIDController
from GeneralController import GeneralController
cur_dir = os.getcwd()
os.sys.path.append(cur_dir + '/../SpringMass/')
from SpringMass import SpringMassVis

#https://gist.github.com/AndrewWalker/2687988
class PendulumVis(SpringMassVis):
	def __init__(self, l = 1.0, m = 1.0, g = 9.8, b = 0.0001):
		i = 1
		self.c= {'x':0.0, 'y': 0.0}
		self.enc_prev = 0
		self.l = l
		self.m = m
		self.g = g
		self.b = b
		self.characteristicEquation()

	def addEncoder(self, conv):
		super(PendulumVis, self).addEncoder(conv)

	def addMotor(self, conv):
		super(PendulumVis, self).addMotor(conv)

	def simplePendulumDynamics(self, x, t, l, g, m, b, u):
		q_dot = np.zeros(2)
		q_dot[0] = x[1]
		gravity = 0.5*(m * g * l * np.sin(x[0]))
		torque = u
		damping = -1 * np.sign(x[1]) * b
		q_dot[1] = gravity + torque + damping;
		return q_dot

	def freeResponse(self, q_zero = [1.0,0.0]): #plots the response from a starting position with no input
		q = np.array(q_zero)
		q_dot = np.zeros(2)
		dt = 0.001
		q_list = []
		time = np.arange(0,10,dt)
		for t in time:
			q_dot = self.simplePendulumDynamics(q, 0, self.l, self.m, self.g, self.b, 0)
			q += q_dot*dt
			q_list.append(copy.deepcopy(q))
		q_list = np.array(q_list)
		f, axx = plt.subplots(1,1)
		for q in q_list[::100]:
			axx.plot([0,-self.l*np.sin(q[0])], [0, self.l*np.cos(q[0])], 'r-')
			axx.plot(-self.l*np.sin(q[0]), self.l*np.cos(q[0]), 'bo')
			plt.draw()
			print(q)
			plt.pause(0.001)
		plt.plot(time, q_list[:,0], label = 'Theta')
		plt.plot(time, q_list[:,1], label = 'ThetaDot')
		plt.legend()
		plt.show()
		pdb.set_trace()

	def characteristicEquation(self):
		#linearized about 0 (which is up)
		s = control.tf([1,0],[1])
		self.CE = 1 / (s**2 - self.b*s + 0.5*self.l*self.m*self.g)

	def plotRootLocus(self):
		super(PendulumVis, self).plotRootLocus()

	def plotStepResponse(self):
		super(PendulumVis, self).plotStepResponse()

	def plotImpulseResponse(self):
		super(PendulumVis, self).plotImpulseResponse()

	# def deltaTSim(self, q_zero, pwm, dt):
	# 	ts = np.linspace(0, dt, 10)
	# 	u = self.Motor.pwm_convert(pwm)
	# 	ys = odeint(self.simplePendulumDynamics, q_zero, ts, args = (self.l, self.g, self.m, self.b, u))
	# 	ys = np.array(ys)
	# 	zero_lim = 1e-20 #zero out anything below this value
	# 	ys[ys<zero_lim] = 0

	# 	return ys[-1,:]

	def deltaTSim(self, q_zero, pwm, dt):
		u = self.Motor.pwm_convert(pwm)
		q = np.array(q_zero)
		q_dot = np.zeros(2)
		q_dot = self.simplePendulumDynamics(q, 0, self.l, self.m, self.g, self.b, u)
		q += q_dot*dt
		return q

	def plotSystem(self, q, u, axx = None, show = True):
		xs = [self.c['x'], -self.l*np.sin(q[0])] #center to tip of pendulum
		ys = [self.c['y'], self.l*np.cos(q[0])]
		if axx is None: f, axx = plt.subplots(1,1)
		axx.hold(False)
		axx.plot(xs, ys, 'b-', linewidth = 3) # plot line
		axx.hold(True)
		axx.plot(xs[1], ys[1], 'ro') #plot pendulum end
		axx.set_xlim([-1.2*self.l, 1.2*self.l])
		axx.set_ylim([-1.2*self.l, 1.2*self.l])
		# axx.set_aspect('square')
		if show:
			plt.show()
		

# class LinearizedPIDController(PIDController): # low level class to position pendulum with PID controller linearized around current point
# 	def __init__(self):
# 		super(LinearizedPIDController, self).__init__()

# 	def linearizeSystem(self, q): # linearize system around current point

class Simulate(object): #class for simulating the whole system
	def __init__(self, system, controller, sim_options = None):
		self.SM = system
		self.Controller = controller
		if not sim_options:
			self.sim = {'dt': 0.001, 't_total': 5.0, 'q_zero': [0.01, 0]}
		else:
			self.sim = sim_options # a dictionary with settings for simulation

	def simulate(self, Plot = False, RTPlot = False):
		t_cur = 0
		q = [self.sim['q_zero']]
		enc = [0]
		u = [0.0]
		u_P = [0.0]
		u_D = [0.0]
		u_I = [0.0]
		e = [0]
		if RTPlot: frt, axrt = plt.subplots(1,1)
		t_plot = 0.1
		t_plot_last = 0
		while t_cur < self.sim['t_total']:
			if (t_cur - self.Controller.last_t) < self.Controller.dt:
				u_cur, e_cur = self.Controller.update(q[-1])
				u.append(u_cur)
				# u_P.append(self.Controller.u_P)
				# u_D.append(self.Controller.u_D)
				# u_I.append(self.Controller.u_I)
				e.append(e_cur)
				self.Controller.last_t = t_cur
			q.append(self.SM.deltaTSim(q[-1], u[-1], self.sim['dt']))
			enc.append(self.SM.Encoder.update(q[-1][0]))
			if t_cur - t_plot_last > t_plot and RTPlot:
				self.SM.plotSystem(q[-1], u[-1], axrt, show = False)
				axrt.plot([self.Controller.q_target[0]]*2, [0, 2], 'g-.')
				plt.draw()
				plt.pause(0.001)
				t_plot_last = t_cur
			t_cur += self.sim['dt']
		if Plot:
			print("Starting to Plot points")
			f, axarr = plt.subplots(3,2)
			axarr[0,1].plot(np.array(q)[:,0], 'r-', label = 'Theta')
			axarr[0,1].plot(np.array(q)[:,1], 'b-', label = 'Theta Dot')
			lim_y = max(abs(np.array(q))[:,0])
			axarr[0,1].plot([0, len(q)], [lim_y, lim_y], 'k-')
			axarr[0,1].plot([0, len(q)], [-lim_y, -lim_y], 'k-')
			axarr[0,1].legend()
			axarr[0,1].set_title('Phase')
			
			axarr[1,1].plot(np.array(enc), 'ko')
			axarr[1,1].set_title('encoder')
			# pdb.set_trace()
			xs = -self.SM.l*np.sin(np.array(q)[:,0])
			ys = self.SM.l*np.cos(np.array(q)[:,0])
			axarr[1,0].plot(xs, ys, 'o')
			axarr[1,0].set_title('Pendulum Position')
			axarr[1,0].set_ylim([-self.SM.l, self.SM.l])
			axarr[1,0].set_xlim([-self.SM.l, self.SM.l])

			axarr[0,0].set_title('Control Params')
			axarr[0,0].plot(np.array(q)[:,0], 'r-', label = 'Theta')
			axarr[0,0].plot(np.array(e), 'g.', label = 'Error')
			axarr[0,0].autoscale(True)
			axarr[0,0].legend()

			axarr[2,0].set_title('Control Input')
			axarr[2,0].autoscale(True)
			axarr[2,0].plot(np.array(u), 'b.', label = 'Input')
			axarr[2,0].legend()
			print('Average Input: %s' %np.mean(np.array(u)))
		steady_state_error = self.Controller.q_target[0] - np.mean(np.array(q[-100:])[:,0])
		return np.array(q)

	def plotPhases(self, phase_tuple, q_targ = None): #phase tuple is (q1, q2, q3) with increasing derivates in each q
		f, axx = plt.subplots(2, 1)
		for i,q in enumerate(phase_tuple):
			axx[0].plot(q[:,0], label = '%s' %i)
			axx[1].plot(q[:,1], label = '%s' %i)
		for i in range(2):
			if q_targ: axx[i].plot([0, len(q[:,i])], [q_targ[i], q_targ[i]], 'k:')
			axx[i].legend()
		plt.show()

	def testControlValues(self): # test a bunch of values and see which one is best
		resp = []
		for p in np.arange(0,100, 10):
			for d in np.arange(0, 100, 10):
				# p = 5.0
				i = 0.0
				# d = 0.0
				self.Controller = PIDController(P = p, I = i, D = d)
				self.Controller.setQTarget([0, 0])
				resp.append(self.simulate())
				print('PID: (%s, %s, %s) has steady state error %s' %(p, i, d, resp[-1]))

class Tests(object):
	def P_stepResponse(self):
		self.P = PendulumVis(l = 1.0, m = 1.0, b = 0.001, g = 9.8)
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 0)
		self.C.sysOpenCE(self.P.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 100); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx)
		self.C.sysRecomp(P = 500); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx)
		self.C.sysRecomp(P = 2000); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx)
		plt.plot([0, max(out[1])], [1, 1], 'k--')

	def PI_RootLocusMultiple(self):
		self.P = PendulumVis(l = 1.0, m = 1.0, b = 0.001, g = 9.8)
		self.C = PIDController(t = 0.01, P = 500, I = 100)
		self.C.sysOpenCE(self.P.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(I = 100); out = control.root_locus(self.C.sysOCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		self.C.sysRecomp(I = 500); out = control.root_locus(self.C.sysOCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, ':')
		self.C.sysRecomp(I = 2000); out = control.root_locus(self.C.sysOCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, '--')

	def PD_StepResponse(self):
		self.P = PendulumVis(l = 1.0, m = 1.0, b = 0.001, g = 9.8)
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 25.6)
		self.C.sysOpenCE(self.P.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 100, D = 10); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 270, D = 27); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 500, D = 50); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 750, D = 75); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		axx.plot([0, max(out[1])], [1, 1], 'k--')

	def PD_ImpulseResponse(self):
		self.P = PendulumVis(l = 1.0, m = 1.0, b = 0.001, g = 9.8)
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 25.6)
		self.C.sysOpenCE(self.P.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 100, D = 10); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 270, D = 27); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 500, D = 50); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 750, D = 75); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
		axx.plot([0, max(out[1])], [1, 1], 'k--')


if __name__ == '__main__':
	PV = PendulumVis(b = 0.5)
	PV.addMotor(1.0/10)
	PV.addEncoder((2*np.pi)/180)
	# PV.freeResponse()
	# PV.plotRootLocus()
	# PV.plotImpulseResponse()
	# PV.plotStepResponse()
	# T = Tests()
	# T.P_stepResponse()
	# T.PI_RootLocusMultiple()
	# T.PD_StepResponse()
	# T.PD_ImpulseResponse()
	q_targ = [0.0, 0.0]
	Ku = 50.0; Tu = 0.000
	# C0 = PIDController(t = 0.01, P = 0, I = 0, D = 0) #Free response
	# C1 = PIDController(t = 0.01, P = -0.6*Ku, I = -Tu/2, D = -Tu/8) #ZN Rule
	C1 = PIDController(t = 0.01, P = -1, I = -.02, D = -100) #ZN Rule
	# C1 = GeneralController(t = 0.01)
	CE = control.tf([1, -10], [1, -1]) * 25
	# C1.setCE(CE)
	# C0.setQTarget(q_targ)
	C1.setQTarget(q_targ)
	sim_o = {'dt': 0.001, 't_total': 25.0, 'q_zero': [np.pi/20, 0]}
	# Sim0 = Simulate(PV, C0, sim_options = sim_o)
	Sim1 = Simulate(PV, C1, sim_options = sim_o)
	# q0 = Sim0.simulate(RTPlot = False, Plot = True)
	q1 = Sim1.simulate(RTPlot = False, Plot = True)
	# Sim1.plotPhases((q1), q_targ)
	# Sim1.plotPhases((q1, q2, q3), q_targ)
	plt.show()
	pdb.set_trace()