import math

import casadi
import matplotlib.pyplot as plt
import pdb
import numpy as np

N = 100  # Control intervals
N_STATES = 2  # [theta, thetad]
U_STATES = 1  # [thetadd]
N_idx = N - 1
LINK_LENGTH = 1.0
DEAD_ZONE = 0.1
FINAL_POSITION = np.pi
G = -2.0


def system_dynamics(x, u):
    thetadd = casadi.cos(x[0] - math.pi/2) * G
    return casadi.vertcat(x[1], u + thetadd)


opti = casadi.Opti()  # optimizaiton problem

q_state = opti.variable(N_STATES, N + 1)  # state trajectory
pos = q_state[0, :]
speed = q_state[1, :]

u = opti.variable(U_STATES, N)  # control variable: acceleration
tf = opti.variable()  # final time

# opti.minimize(tf)  # Objective is to minimize the total time
# opti.minimize(casadi.mtimes(u, casadi.transpose(u)))  # Minimize acceleration
opti.minimize(casadi.mtimes(casadi.diff(u), casadi.transpose(casadi.diff(u))))  # Minimize jerk
# opti.minimize(casadi.mtimes(casadi.diff(u), casadi.transpose(casadi.diff(u))) + tf/20)  # Minimize jerk and time

dt = tf / N  # Each control interval
for k in range(N):
    # RK4 method
    k1 = system_dynamics(q_state[:, k], u[:, k])
    k2 = system_dynamics(q_state[:, k] + dt / 2 * k1, u[:, k])
    k3 = system_dynamics(q_state[:, k] + dt / 2 * k2, u[:, k])
    k4 = system_dynamics(q_state[:, k] + dt * k3, u[:, k])
    x_next = q_state[:, k] + dt / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4)
    opti.subject_to(q_state[:, k + 1] == x_next)  # reduce the defect!

# Path Conditions
opti.subject_to(-1 <= u)
opti.subject_to(u <= 1)


def convert_joint_space_to_cartesian(p):
    x = LINK_LENGTH*casadi.cos(p[0] + math.pi / 2)
    y = LINK_LENGTH*casadi.sin(p[0] + math.pi/2)
    return x, y


# boundary conditions
opti.subject_to(pos[0] == 0)
opti.subject_to(speed[0] == 0)
opti.subject_to(u[0] == 0)
opti.subject_to(casadi.fabs(casadi.fmod(pos[N_idx+1], 2*np.pi)) == FINAL_POSITION)  # finish at final pose but include wrapping!
opti.subject_to(speed[N_idx+1] == 0)  # finish at 0 velocity

opti.subject_to(tf >= 0)  # time has to be positive
opti.subject_to(tf <= 8.0)  # bang bang min is 3.9

# initial guess
# linear_position_guess = np.linspace(0, FINAL_POSITION, N+1)
linear_position_guess = np.cos(np.linspace(0, 2*FINAL_POSITION, N+1)) * FINAL_POSITION
# opti.set_initial(pos, linear_position_guess)
opti.set_initial(u, np.cos(np.linspace(0, 2*np.pi, N)))

opti.solver('ipopt')

def debug_plot(i):
    # plt.plot(i, opti.debug.value(tf), 'x', label='{}'.format(i))
    # plt.plot(opti.debug.value(q_state)[0, :], 'x', label='{}'.format(i))
    plt.plot(opti.debug.value(u)[:], 'x', label='{}'.format(i))
    plt.draw()
    plt.pause(0.0001)
opti.callback(lambda i: debug_plot(i))
try:
    sol = opti.solve()
except Exception as e:
    plt.legend()
    pdb.set_trace()
    raise e

print('Total Time: {}'.format(sol.value(tf)))
f, axs = plt.subplots(3, 2)
axs[0, 0].plot(sol.value(q_state)[0, :], label='position')

axs[1, 0].plot(sol.value(q_state)[1, :], 'rx-', label='theta-speed')

axs[2, 0].plot(sol.value(u)[:], 'rx-', label='control')
plt.show(block=True)
