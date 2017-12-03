#!/usr/bin/env python
import numpy as np
import matplotlib.pyplot as plt
import pdb
from scipy.integrate import odeint
import os
cur_dir = os.getcwd()
os.sys.path.append(cur_dir + '/../ControlsGeneral/')
from Encoder import Encoder
from Motor import Motor
from PIDController import PIDController
import control
import random


class SpringMassVis(object):
	def __init__(self, m = 1.0, l = 1.0, k = 40, g = 9.8, b = 8.8):
		self.c = {'x': 0.0, 'y': 0.0}
		self.m = m
		self.l = l
		self.k = k
		self.g = g
		self.b = b
		self.characteristicEquation()

	def addEncoder(self, conv):
		self.Encoder = Encoder(conv)

	def addMotor(self, conv):
		self.Motor = Motor(conv)

	def simpleSpringMassDynamics(self, x, t, l, g, m, k, b, u):
		q_dot = np.zeros(2)
		q_dot[0] = x[1]
		# gravity = - (m * g)
		springForce = -(x[0] - (self.c['x'] + l)) * k
		damping = -1 * np.sign(x[1]) * b
		input_force = u
		q_dot[1] = springForce + damping + input_force
		return q_dot

	def characteristicEquation(self):
		s = control.tf([1,0], [1])
		self.CE = 1 / (self.m * s**2 + self.b * s + self.k)

	def plotRootLocus(self):
		control.root_locus(self.CE)
		plt.axvline(x=0, color='k', lw=1)
		plt.grid(True, which='both')
		plt.show()

	def plotStepResponse(self):
		t, q = control.step_response(self.CE)
		plt.plot(t, q)
		plt.title('Step Response')
		plt.show()

	def plotImpulseResponse(self):
		t, q = control.impulse_response(self.CE)
		plt.plot(t,q)
		plt.show()

	def deltaTSim(self, q_zero, pwm, dt):
		ts = np.linspace(0, dt, 3)
		u = self.Motor.pwm_convert(pwm)
		ys = odeint(self.simpleSpringMassDynamics, q_zero, ts, args = (self.l, self.g, self.m, self.k, self.b, u))
		return ys[-1,:]

	def deltaTSim(self, q_zero, pwm, dt): #this works in this case!
		u = self.Motor.pwm_convert(pwm)
		q_dot = self.simpleSpringMassDynamics(q_zero, 0, self.l, self.g, self.m, self.k, self.b, u)
		ys = q_zero + q_dot * dt
		return ys

	def plotSystem(self, x, u, axx = None, show = True):
		mass_w = 1

		if axx is None:
			f, axx = plt.subplots(1,1)
		surface = axx.plot([self.c['x'], 10], [self.c['y'], self.c['y']], '-k')
		wall = axx.plot([self.c['x'], self.c['y']], [self.c['x'], 2], '-k')
		spring_x = np.linspace(self.c['x'], x[0], 10)
		spring_y = [0.5, 0.5, 0.25, 0.75, 0.25, 0.75, 0.25, 0.75, 0.5, 0.5]
		spring = axx.plot(spring_x, spring_y)
		natural_l = axx.plot([self.l, self.l], [0, 2], 'b--')
		mass_x = [x[0], x[0], x[0] + mass_w, x[0] + mass_w,x[0]]
		mass_y = [0, mass_w, mass_w, 0, 0]
		mass = axx.plot(mass_x, mass_y, 'k-', linewidth = 3)
		input_arrow = axx.arrow(x[0], mass_w*1.25, u/255.0*2, 0, head_width=0.05, head_length=0.1, fc='b', ec='b')
		if show:
			plt.show()
		# pdb.set_trace()



class AnalyzeSystem(object):
	def P_analyze(self, system, controller):
		self.S = system
		self.C = controller

		self.C.sysOpenCE(self.S.CE)
		print("Open Loop: %s" %self.C.sysOCE)
		self.C.sysClosedCE()
		print("Closed Loop: %s" %self.C.sysCCE)
		
		# plot open loop Bode
		self.C.sysRecomp(P = 100); OC1 = self.C.sysOpenCE(self.S.CE)
		self.C.sysRecomp(P = 500); OC2 = self.C.sysOpenCE(self.S.CE)
		self.C.sysRecomp(P = 2000); OC3 = self.C.sysOpenCE(self.S.CE)
		control.bode([OC1, OC2, OC3])
		# plot step response
		# plot closed loop Bode

	def PI_analyze(self, system, controller):
		self.S = system
		self.C = controller

		self.C.sysOpenCE(self.S.CE)
		print("Open Loop: %s" %self.C.sysOCE)
		self.C.sysClosedCE()
		print("Closed Loop: %s" %self.C.sysCCE)

		self.C.sysRecomp(P = 10, I = 30); OC1 = self.C.sysOpenCE(self.S.CE)
		self.C.sysRecomp(P = 50, I = 150); OC2 = self.C.sysOpenCE(self.S.CE)
		self.C.sysRecomp(P = 200, I = 600); OC3 = self.C.sysOpenCE(self.S.CE)
		control.bode([OC1, OC2, OC3])



	# def root_locus(self, l_func): # l_func is a lambda function with one input for the transfer function
	# 	_start = control.pole(l_fun(0))
	# 	_end = control.pole(l_func(1000))
	# 	plt.figure()
	# 	plt.plot(real(_start), )

class Tests(object):
	def __init__(self):
		i = 1

	def P_StepResponse(self):
		self.SM = SpringMassVis(l = 0.0, m = 1, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 0)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 100); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx)
		self.C.sysRecomp(P = 500); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx)
		self.C.sysRecomp(P = 2000); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx)
		plt.plot([0, max(out[1])], [1, 1], 'k--')

	def PI_RootLocusMultiple(self):
		self.SM = SpringMassVis(l = 1.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 100)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(I = 100); out = control.root_locus(self.C.sysOCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		self.C.sysRecomp(I = 500); out = control.root_locus(self.C.sysOCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, ':')
		self.C.sysRecomp(I = 2000); out = control.root_locus(self.C.sysOCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, '--')

	def PI_ImpulseResponse(self):
		self.SM = SpringMassVis(l = 1.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 100)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 25, I = 75); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 50, I = 150); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 75, I = 225); out = self.C.plotImpulseResponse(self.C.sysCCE, Plot = False, axx = axx);
	
	def PI_StepResponse(self):
		self.SM = SpringMassVis(l = 1.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 100)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 25, I = 75); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 50, I = 150); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 75, I = 225); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		axx.plot([0, max(out[1])], [1, 1], 'k--')

	def PD_RootLocusMultiple(self):
		self.SM = SpringMassVis(l = 1.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 25.6)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 100, D = 10); out = control.root_locus(self.C.sysCCE/self.C.D, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		# self.C.sysRecomp(P = 250, D = 25); out = control.root_locus(self.C.sysCCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, ':')
		# self.C.sysRecomp(P = 1000, D = 100); out = control.root_locus(self.C.sysCCE/self.C.I, Plot = False); self.C.plotRootLocus(out, axx, '--')

	def PD_StepResponse(self):
		self.SM = SpringMassVis(l = 0.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 25.6)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 100, D = 10); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 270, D = 27); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 500, D = 50); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		self.C.sysRecomp(P = 750, D = 75); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		axx.plot([0, max(out[1])], [1, 1], 'k--')

	def PID_RootLocusMultiple(self):
		self.SM = SpringMassVis(l = 1.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 25.6)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P = 15, I = 50, D = 1); out = control.root_locus(self.C.sysCCE/self.C.D, Plot = False); self.C.plotRootLocus(out, axx, 'x')

	def PID_StepResponse(self):
		self.SM = SpringMassVis(l = 0.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 25.6)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		D = 5; self.C.sysRecomp(P=15*D, I=50*D, D=D); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		D = 10; self.C.sysRecomp(P=15*D, I=50*D, D=D); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		D = 15; self.C.sysRecomp(P=15*D, I=50*D, D=D); out = self.C.plotStepResponse(self.C.sysCCE, Plot = False, axx = axx);
		axx.plot([0, max(out[1])], [1, 1], 'k--')

	def RLChange(self):
		self.SM = SpringMassVis(l = 0.0, k = 40.0, b = 8.80) #cm
		self.C = PIDController(t = 0.01, P = 50, I = 0, D = 0)
		self.C.sysOpenCE(self.SM.CE)
		self.C.sysClosedCE()
		f, axx = plt.subplots(1,1)
		self.C.sysRecomp(P=1, I=0, D=0); out=control.root_locus(self.C.sysCCE/self.C.P, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		self.C.sysRecomp(P=1, I=3, D=0); out=control.root_locus(self.C.sysCCE/self.C.P, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		self.C.sysRecomp(P=1, I=0, D=0.1); out=control.root_locus(self.C.sysCCE/self.C.D, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		self.C.sysRecomp(P=1, I=5, D=0.1); out=control.root_locus(self.C.sysCCE/self.C.D, Plot = False); self.C.plotRootLocus(out, axx, 'x')
		control.root_locus(self.C.sysCCE/self.C.D, Plot = True)

	def P_Bode(self):
		s = control.tf([1,0], [1])
		CE = 1/(s**2 + 8.8*s + 40)
		P = control.tf([1],[1])
		OL = P * CE
		CL = control.feedback(OL, 1, sign = -1)
		control.root_locus(CL, Plot = True)
		plt.figure()
		P = control.tf([100], [1]); OL1 = P * CE; CL1 = control.feedback(OL1, 1, sign = -1)
		P = control.tf([500], [1]); OL2 = P * CE; CL2 = control.feedback(OL2, 1, sign = -1)
		P = control.tf([2000], [1]); OL3 = P * CE; CL3 = control.feedback(OL3, 1, sign = -1)
		print("Closed Loop")
		print([CL1, CL2, CL3])
		control.bode_plot([OL1, OL2, OL3], Plot = True)
		
		print(CL)

class Simulate(object):
	def __init__(self, system, controller, sim_options = None):
		self.SM = system
		self.Controller = controller
		if not sim_options:
			self.sim = {'dt': 0.001, 't_total': 5.0, 'q_zero': [11.0, 0]}
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
		t_plot = 0.1
		t_plot_last = 0
		if RTPlot: f, axx = plt.subplots(1,1)
		while t_cur < self.sim['t_total']:
			if (t_cur - self.Controller.last_t) < self.Controller.dt:
				u_cur, e_cur = self.Controller.update(q[-1])
				u.append(u_cur)
				u_P.append(self.Controller.u_P)
				u_D.append(self.Controller.u_D)
				u_I.append(self.Controller.u_I)
				e.append(e_cur)
				self.Controller.last_t = t_cur
			q.append(self.SM.deltaTSim(q[-1], u[-1], self.sim['dt']))
			enc.append(self.SM.Encoder.update(q[-1][0]))
			if t_cur - t_plot_last > t_plot and RTPlot:
				self.SM.plotSystem(q[-1], u[-1], axx, show = False)
				axx.plot([self.Controller.q_target[0]]*2, [0, 2], 'g-.')
				plt.draw()
				plt.pause(0.001)
				axx.cla()
				t_plot_last = t_cur
			t_cur += self.sim['dt']
		if Plot:
			f_r, axx_r = plt.subplots(2,1)
			axx_r[0].plot(np.array(q)[:,0], 'k:', label = 'q')
			axx_r[0].plot(np.array(q)[:,1], 'b--', label = 'q_dot')
			axx_r[0].set_title('Phase')
			axx_r[0].legend()
			axx_r[0].grid(color = 'grey')
			axx_r[1].plot(np.array(u_P), 'y:', label = 'P')
			axx_r[1].plot(np.array(u_D), 'r:', label = 'D')
			axx_r[1].plot(np.array(u_I), 'g:', label = 'I')
			# axx_r[1].plot(np.array(u), 'b:', label = 'F_in')
			axx_r[1].set_title('Control')
			axx_r[1].grid(color = 'grey')
			axx_r[1].legend()
			plt.show()
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


if __name__ == '__main__':
	SM = SpringMassVis(l = 10.0, k = 40.0, b = 8.80) #cm
	SM.addMotor(0.1)
	SM.addEncoder((2 * np.pi) / 180.0)

	# SM.plotSystem([2.0,0], 0, axx = None)
	# SM.plotRootLocus()
	# pdb.set_trace()
	# SM.plotStepResponse()
	# SM.plotImpulseResponse()
	# Sim = Simulate()
	# Sim.simulate(RTPlot = True)
	T = Tests()
	# T.PI_RootLocusMultiple()
	# T.PI_StepResponse()
	# T.PI_ImpulseResponse()
	# T.PD_RootLocusMultiple()
	# T.PD_StepResponse()
	# T.PID_RootLocusMultiple()
	# T.PID_StepResponse()
	# T.RLChange()
	# T.P_Bode()

	A = AnalyzeSystem()

	q_targ = [9.0, 0.0]
	Ku = 2.15e2; Tu = 0.4
	C1 = PIDController(t = 0.01, P = -0.6*Ku, I = -Tu/2, D = -Tu/8) #ZN Rule
	C1.setQTarget(q_targ)
	C2 = PIDController(t = 0.01, P = -0.5*Ku, I = -0, D = 0) #P
	C2.setQTarget(q_targ)
	C3 = PIDController(t = 0.01, P = -0.2*Ku, I = -Tu/2, D = -Tu/3) # no overshoot
	C3.setQTarget(q_targ)
	sim_o = {'dt': 0.001, 't_total': 5.0, 'q_zero': [10.0, 0]}
	Sim1 = Simulate(SM, C1, sim_options = sim_o)
	Sim2 = Simulate(SM, C2, sim_options = sim_o)
	Sim3 = Simulate(SM, C3, sim_options = sim_o)
	# A.PI_analyze(SM, C)
	q1 = Sim1.simulate(RTPlot = False)
	q2 = Sim2.simulate(RTPlot = False)
	q3 = Sim3.simulate()
	Sim1.plotPhases((q1, q2, q3), q_targ)