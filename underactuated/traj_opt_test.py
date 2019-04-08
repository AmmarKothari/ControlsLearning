from abc import ABCMeta, abstractmethod

import matplotlib.pyplot as plt
import pdb
import numpy as np
from functools import partial
import scipy.optimize as so



# trajectory optimizaiton for a 1D double integrator

def dynamics(q, u, dt):
    """
    forward dynamics in time
    :param q: state
    :param u: control input
    :param dt: time step
    :return: new state
    """
    q_next = np.zeros(2)
    q_next[0] = q[0] + dt*q[1] + 1/2*dt**2*u
    q_next[1] = q[1] + dt*u

    return q_next


class DynamicOptmizer(object):
    __metaclass__ = ABCMeta

    def __init__(self):
        self.result = None
        self.traj = None

    def unpack(self, min_arg, knot_points):
        x = min_arg[0:self.dims*knot_points].reshape(-1, self.dims)
        u = min_arg[self.dims*knot_points:]
        return x, u

    def pack(self, x, u):
        return np.vstack((x, u))

    @abstractmethod
    def dynamics(self, dt, q, u):
        pass

    def state_error(self, x, traj):
        return np.linalg.norm(traj[1:, 1:(self.dims+1)] - x)

    @abstractmethod
    def integ_error(self, u, traj, dt):
        pass

    def cost_func(self, knot_points, traj, dt, v):
        # Note: this is an implicit approach because there are no constrains only costs!
        # TODO: figure out how to handle start state and end control cleanly
        x, u = self.unpack(v, knot_points)
        state_error = self.state_error(x, traj)
        integration_error = self.integ_error(u, traj, dt)

        return integration_error + state_error

    def run_optimization(self, traj_func, cost_func, t_range, knot_points, dims):
        self.dims = dims
        x0 = np.zeros(knot_points * (self.dims+1))
        dt = (t_range[1] - t_range[0])/float(knot_points)
        self.traj = traj_func(t_range, knot_points)
        minimize_func = partial(cost_func, knot_points, self.traj, dt)
        print('Initial Cost: {}'.format(minimize_func(x0)))

        self.result = so.minimize(minimize_func, x0)
        print('Final Cost: {}'.format(minimize_func(self.result.x)))


class Vone(DynamicOptmizer):
    # decision variables are x1...xn, u0...un-1

    def dynamics(self, dt, q, u):
        # u is velocity control
        # q is just the position
        return q[0:-1, 1] + dt*u

    def integ_error(self, u, traj, dt):
        x_control = self.dynamics(dt, traj, u)
        return np.linalg.norm(x_control - traj[1:, 1])

    def sin_traj(self, t_range, num_points):
        """
        sinusoidal position trajectory
        :param t_range:
        :return: series of tuples (time, position)
        """
        traj = []
        for t in np.linspace(t_range[0], t_range[-1], num_points+1):
            traj.append((t, np.sin(t)))

        return np.array(traj)

    def plot(self):
        f, axs = plt.subplots(2, sharex=True)
        # position
        axs[0].plot(self.traj[:, 0], self.traj[:, 1], 'b:')
        axs[0].set_title('Position')
        x_res, u_res = self.unpack(self.result.x, knot_points)
        axs[0].plot(self.traj[1:, 0], x_res, 'rx-')

        axs[1].plot(self.traj[0:-1, 0], u_res[:], 'rx-')
        axs[1].set_title('Control')
        plt.show()


class Vtwo(DynamicOptmizer):
        # decision variables are x1...xn, xd1...xdn, u0...un-1

    def sin_traj(self, t_range, num_points):
        traj = []
        for t in np.linspace(t_range[0], t_range[-1], num_points+1):
            traj.append((t, np.sin(t), np.cos(t)))
        return np.array(traj)

    def dynamics(self, dt, q, u):
        pass

    def integ_error(self, u, traj, dt):
        pdb.set_trace()

    def plot(self):
        pass




knot_points = 100
t_range = [0, 10]
dims = 2
opt = Vtwo()
opt.run_optimization(opt.sin_traj, opt.cost_func, t_range, knot_points, dims)
opt.plot()

pdb.set_trace()



