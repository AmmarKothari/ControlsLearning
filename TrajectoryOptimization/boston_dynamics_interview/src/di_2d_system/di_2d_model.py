import casadi


class DI2D:

    def __init__(self, n_control_step, num_states, num_controls):
        self.num_control_steps = n_control_step
        self.num_states = num_states
        self.num_controls = num_controls
        self.opti = casadi.Opti()
        self.setup_vars()
        self.setup_defect()

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
        for i, lower_u in enumerate(min_u):
            self.opti.subject_to(lower_u <= self.u[0, :])
            self.opti.subject_to(lower_u <= self.u[1, :])
        for iiupper_u in enumerate(max_u):
            self.opti.subject_to(self.u[0, :] <= upper_u)
            self.opti.subject_to(self.u[1, :] <= upper_u)
