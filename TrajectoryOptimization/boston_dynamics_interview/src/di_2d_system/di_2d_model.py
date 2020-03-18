import enum

import casadi
from typing import List
import matplotlib.pyplot as plt
from typing import Optional


class ObjectiveTypes(enum.Enum):
    MINIMIZE_TIME = 1
    MINIMIZE_ACCELERATION = 2
    MINIMIZE_JERK = 3


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



class DI2D:

    def __init__(self, n_control_step, num_states=4, num_controls=2):
        self.num_control_steps = n_control_step
        self.num_states = num_states
        self.num_controls = num_controls
        self.opti = casadi.Opti()
        self.setup_vars()
        self.setup_defect()
        self.sol = None

    @property
    def max_control_idx(self):
        return self.num_control_steps - 1

    @property
    def num_state_steps(self):
        return self.num_control_steps + 1

    @property
    def max_state_idx(self):
        return self.num_control_steps - 1

    def system_dynamics(self, x, u):
        return casadi.vertcat(x[2], x[3], u[0], u[1])

    def setup_vars(self):
        self.q_state = self.opti.variable(self.num_states, self.num_state_steps)
        self.u = self.opti.variable(self.num_controls, self.num_control_steps)
        self.tf = self.opti.variable()

    def setup_defect(self):
        dt = self.tf / self.num_control_steps  # Each control interval
        for k in range(self.num_control_steps):
            # RK4 method
            k1 = self.system_dynamics(self.q_state[:, k], self.u[:, k])
            k2 = self.system_dynamics(self.q_state[:, k] + dt / 2 * k1, self.u[:, k])
            k3 = self.system_dynamics(self.q_state[:, k] + dt / 2 * k2, self.u[:, k])
            k4 = self.system_dynamics(self.q_state[:, k] + dt * k3, self.u[:, k])
            x_next = self.q_state[:, k] + dt / 6.0 * (k1 + 2 * k2 + 2 * k3 + k4)
            self.opti.subject_to(self.q_state[:, k + 1] == x_next)  # reduce the defect!

    def setup_control_constraints(self, max_u, min_u):
        # type: (List[float], List[float]) -> None
        for i_u in range(self.num_controls):
            for lower_u in min_u:
                self.opti.subject_to(lower_u <= self.u[i_u, :])
            for upper_u in max_u:
                self.opti.subject_to(self.u[i_u, :] <= upper_u)

    def setup_start_constraints(self, start_q, start_u):
        # type: (List[float], List[float]) -> None
        for i in range(self.num_states):
            self.opti.subject_to(self.q_state[i, 0] == start_q[i])
        for i in range(self.num_controls):
            self.opti.subject_to(self.u[i, 0] == start_u[i])

    def setup_end_constraints(self, end_q, end_u=None):
        # type: (List[float], Optional[List[float]]) -> None
        for i in range(self.num_states):
            self.opti.subject_to(self.q_state[i, -1] == end_q[i])
        if end_u is not None:
            for i in range(self.num_controls):
                    self.opti.subject_to(self.u[i, -1] == end_u[i])

    def constrain_time(self, max_time=None):
        self.opti.subject_to(self.tf >= 0)
        if max_time is not None:
            self.opti.subject_to(self.tf <= max_time)

    def set_initial_guess(self, q_guess=None, u_guess=None):
        if q_guess is not None:
            self.opti.set_initial(self.q_state, q_guess)
        if u_guess is not None:
            self.opti.set_initial(self.u, u_guess)

    def setup_solve(self):
        self.opti.solver('ipopt')

    def solve(self):
        self.sol = self.opti.solve()

    def set_objective(self, objective_type):
        if objective_type == ObjectiveTypes.MINIMIZE_TIME:
            self.opti.minimize(self.tf)
        elif objective_type == ObjectiveTypes.MINIMIZE_ACCELERATION:
            self.opti.minimize(minimize_acceleration(self.u))
        elif objective_type == ObjectiveTypes.MINIMIZE_JERK:
            self.opti.minimize(minimize_jerk(self.u))

    def plot_solution(self):
        f, axs = plt.subplots(3, 2)
        axs[0, 0].plot(self.sol.value(self.q_state)[0, :], self.sol.value(self.q_state)[1, :], label='position')
        # axs[0, 0].plot(FINAL_POSITION[0], FINAL_POSITION[1], 'go', label='goal')

        axs[1, 0].plot(self.sol.value(self.q_state)[2, :], 'rx-', label='x-speed')
        axs[1, 1].plot(self.sol.value(self.q_state)[3, :], 'bx-', label='y-speed')
        #
        axs[2, 0].plot(self.sol.value(self.u)[0, :], 'rx-', label='control')
        axs[2, 1].plot(self.sol.value(self.u)[1, :], 'bx-', label='control')
        plt.show(block=True)


    # def debug_u_plot(self):


