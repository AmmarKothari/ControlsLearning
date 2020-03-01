import casadi
import matplotlib.pyplot as plt
import pdb
import numpy as np

N = 100 # Control intervals
N_idx = N - 1
SLIDING_FRICTION = 0.1
DEAD_ZONE = 0.5
FINAL_POSITION = 10

def system_dynamics(x, u):
    # u_dead_zone = casadi.if_else(casadi.fabs(u) <= DEAD_ZONE, 0.0, u)
    # acceleration = u_dead_zone - x[1] * SLIDING_FRICTION
    return casadi.vertcat(x[1], u)


opti = casadi.Opti()  # optimizaiton problem

q_state = opti.variable(2, N + 1)  # state trajectory
pos = q_state[0, :]
speed = q_state[1, :]

u = opti.variable(1, N)  # control variable: acceleration
tf = opti.variable()  # final time

# opti.minimize(tf)  # Objective is to minimize the total time
# opti.minimize(casadi.mtimes(u, casadi.transpose(u)))  # Minimize acceleration
# opti.minimize(casadi.mtimes(casadi.diff(u), casadi.transpose(casadi.diff(u))))  # Minimize jerk
opti.minimize(casadi.mtimes(casadi.diff(u), casadi.transpose(casadi.diff(u))) + tf/20)  # Minimize jerk and time
# Should result in bang bang control

dt = tf / N  # Each control interval
for k in range(N):
    # RK4 method
    k1 = system_dynamics(q_state[:, k], u[:, k])
    k2 = system_dynamics(q_state[:, k] + dt / 2 * k1, u[:, k])
    k3 = system_dynamics(q_state[:, k] + dt / 2 * k2, u[:, k])
    k4 = system_dynamics(q_state[:, k] + dt * k3, u[:, k])
    x_next = q_state[:, k] + dt / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4)
    opti.subject_to(q_state[:, k + 1] == x_next)  # reduce the defect!

# for k in range(N):
#     integrator = casadi.integrator('integrator', 'cvodes', dae, {'grid':ts, 'output_t0':True})


# path constraints
opti.subject_to(-1<=u)
opti.subject_to(u<=1)


# boundary conditions
opti.subject_to(pos[0]==0)
opti.subject_to(speed[0] == 0)
opti.subject_to(u[0] == 0)
opti.subject_to(pos[0, N_idx+1] == FINAL_POSITION)  # finish at 10
opti.subject_to(speed[0, N_idx+1] == 0)  # finish at 0 velocity

opti.subject_to(tf >= 0)  # time has to be positive
# opti.subject_to(tf <= 8.0)  # bang bang min is 6.4

# initial guess
linear_position_guess = np.linspace(0, FINAL_POSITION, N+1)
opti.set_initial(pos, linear_position_guess)
# opti.set_initial(speed, np.linspace(0, FINAL_POSITION, N+1))
# opti.set_initial(u, np.linspace(0, 1, N))
# opti.set_initial(tf, 10)

opti.solver('ipopt')
# plt.figure()
# plt.show(block=False)
# opti.callback(lambda i: plt.plot(opti.debug.value(tf), 'x', label='{}'.format(i)))
try:
    sol = opti.solve()
except Exception as e:
    # plt.legend()
    pdb.set_trace()
    raise e
print('Total Time: {}'.format(sol.value(tf)))
f, axs = plt.subplots(3, 1)
axs[0].plot(sol.value(pos), label='position')
axs[0].axhline(10, linestyle=':')

axs[1].plot(sol.value(speed), label='speed')

axs[2].plot(sol.value(u), 'rx-', label='control')
plt.show()



