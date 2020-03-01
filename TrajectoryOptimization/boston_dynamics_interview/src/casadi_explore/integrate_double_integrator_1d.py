import casadi
import numpy as np
import matplotlib.pyplot as plt


CONSTANT_ACCELERATION = 0.1
TF = 10*np.pi


t = casadi.SX.sym('t')

q = casadi.SX.sym('q')
qd = casadi.SX.sym('qd')

state = casadi.vertcat(q, qd)

rhs = casadi.vertcat(qd, casadi.sin(t))

dae = {'x': state, 't': t, 'ode': rhs}
ts = np.linspace(0, TF, 10000)

integrator = casadi.integrator('integrator', 'cvodes', dae, {'grid':ts, 'output_t0':True})

sol = integrator(x0=[0, -1.0])

import pdb
# pdb.set_trace()
sol_q = np.array(sol['xf'][0, :]).flatten()
sol_qd = np.array(sol['xf'][1, :]).flatten()

plt.plot(ts, sol_q, 'r-', label='Position')
plt.plot(ts, sol_qd, 'b-', label='Velocity')
ax = plt.gca()
ax.axhline(0)

plt.legend()
plt.show()
