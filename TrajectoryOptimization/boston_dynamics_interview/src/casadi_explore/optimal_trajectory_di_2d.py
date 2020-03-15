import casadi
import matplotlib.pyplot as plt
import pdb
import numpy as np
import shapely.geometry as sg

"""
state vector = [x, y, xd, yd]
state_d vector = [xd, yd, xdd, ydd]
control vector = [xdd, ydd]
"""


num_control_steps = 100
max_control_idx = num_control_steps - 1
num_state_steps = num_control_steps + 1
max_state_idx = num_state_steps - 1

num_states = 4
num_controls = 2

FINAL_POSITION = [10, 10]


def system_dynamics(x, u):
    return casadi.vertcat(x[2], x[3], u[0], u[1])


def minimize_acceleration(u):
    return casadi.sum2(casadi.sum1(casadi.mtimes(u, casadi.transpose(u))))


def minimize_jerk(u):
    xdd = u[0, :]
    ydd = u[1, :]
    xddd = casadi.diff(xdd)
    yddd = casadi.diff(ydd)
    xddd_squared = casadi.mtimes(xddd, casadi.transpose(xddd))
    yddd_squared = casadi.mtimes(yddd, casadi.transpose(yddd))
    return xddd_squared + yddd_squared

opti = casadi.Opti()

q_state = opti.variable(num_states, num_state_steps)

u = opti.variable(2, num_control_steps)
tf = opti.variable()



# Objective Functions
# opti.minimize(tf)
# opti.minimize(minimize_acceleration(u))
opti.minimize(minimize_jerk(u))

dt = tf / num_control_steps  # Each control interval
for k in range(num_control_steps):
    # RK4 method
    k1 = system_dynamics(q_state[:, k], u[:, k])
    k2 = system_dynamics(q_state[:, k] + dt / 2 * k1, u[:, k])
    k3 = system_dynamics(q_state[:, k] + dt / 2 * k2, u[:, k])
    k4 = system_dynamics(q_state[:, k] + dt * k3, u[:, k])
    x_next = q_state[:, k] + dt / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4)
    opti.subject_to(q_state[:, k + 1] == x_next)  # reduce the defect!


# Path Conditions
opti.subject_to(-1 <= u[0, :])
opti.subject_to(-1 <= u[1, :])
opti.subject_to(u[0, :] <= 1)
opti.subject_to(u[1, :] <= 1)

# no states within a region
no_go_region = sg.Polygon(([-5, -5], [-5, 5], [5, 5], [5, -5]))
# for k in range(num_state_steps):
#     pdb.set_trace()
#     opti.subject_to(no_go_region.contains(sg.Point(q_state[:2, k])))

# Boundary Conditions
opti.subject_to(q_state[:, 0] == 0)  # Everything starts at zero
opti.subject_to(u[:, 0] == 0)  # No control input at the start

opti.subject_to(q_state[2:, -1] == 0)  # Zero velocity at the end
opti.subject_to(q_state[0, -1] == FINAL_POSITION[0])
opti.subject_to(q_state[1, -1] == FINAL_POSITION[1])
opti.subject_to(u[:, -1] == 0)  # No control input at the end

opti.subject_to(tf >= 0)
opti.subject_to(tf <= 10)

# Initial Guesses
opti.set_initial(q_state[0, :], np.linspace(0, FINAL_POSITION[0], num_state_steps))
opti.set_initial(q_state[1, :], np.linspace(0, FINAL_POSITION[1], num_state_steps))
opti.set_initial(u[0, :], 1)
opti.set_initial(u[1, :], 1)
opti.set_initial(tf, 10)

# Assume a not-optimal collision free path
# You can check that each point has x free space around it (i.e. collision check up to 1 m away)
# use that value to constrain how much each point in the path can move?
# Use that solution to find a new better solution


opti.solver('ipopt')
pdb.set_trace()
# plt.figure()
# plt.ion()
# plt.show()
# plt.draw()
def debug_plot(i):
    # plt.plot(i, opti.debug.value(tf), 'x', label='{}'.format(i))
    # plt.plot(opti.debug.value(q_state)[0, :], 'x', label='{}'.format(i))
    plt.plot(opti.debug.value(u)[0, :], 'x', label='{}'.format(i))
    plt.draw()
    plt.pause(0.0001)
    # plt.sleep(0.0001)
# opti.callback(lambda i: debug_plot(i))
# opti.callback(lambda i: print('Current iteration: {}'.format(i)))
try:
    sol = opti.solve()
except Exception as e:
    plt.show()
    pdb.set_trace()
    raise e
print('Total Time: {}'.format(sol.value(tf)))
f, axs = plt.subplots(3, 2)
axs[0, 0].plot(sol.value(q_state)[0, :], sol.value(q_state)[1, :], label='position')
axs[0, 0].plot(FINAL_POSITION[0], FINAL_POSITION[1], 'go', label='goal')

axs[1, 0].plot(sol.value(q_state)[2, :], 'rx-', label='x-speed')
axs[1, 1].plot(sol.value(q_state)[3, :], 'bx-', label='y-speed')
#
axs[2, 0].plot(sol.value(u)[0, :], 'rx-', label='control')
axs[2, 1].plot(sol.value(u)[1, :], 'bx-', label='control')
plt.show(block=True)