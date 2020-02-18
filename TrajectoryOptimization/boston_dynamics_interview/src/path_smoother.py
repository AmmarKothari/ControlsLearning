from typing import Callable
from typing import Sequence
from typing import Tuple

import numpy as np
import scipy.optimize


class SmootherException(Exception):
    pass


class PathSmootherConfig:
    def __init__(self, n_states, n_control, t_boundary, state_boundary, n_grid, t_grid, h_step, model):
        self.n_states = n_states
        self.n_control = n_control
        self.t_boundary = t_boundary
        self.state_boundary = state_boundary
        self.n_grid = n_grid
        self.t_grid = t_grid
        self.h_step = h_step
        self.model = model


class PathSmoother:
    def __init__(self, num_states, num_control_states, dyn_func):
        self.num_states = num_states
        self.num_control_states = num_control_states
        self.dyn_func = dyn_func
        self.initial_states = None
        self.initial_controls = None
        self.start_state = None
        self.end_state = None

    def set_initial_guess(self, initial_states, initial_controls):
        self.initial_states = initial_states
        self.initial_controls = initial_controls

    def set_boundary_states(self, start_state, end_state):
        self.start_state = start_state
        self.end_state = end_state

    def smooth_single_shooting(self, initial_guess, obj_fun, bounds=None):
        """Variation in the control to get from the start state to the end state"""
        constraints = []
        for i_state in range(self.num_states):
            constraints.append({'type': 'eq', 'fun': lambda x: self.get_boundary_constraint(x, self.start_state, 0, i_state)})
            constraints.append({'type': 'eq', 'fun': lambda x: self.get_boundary_constraint(x, self.end_state, -1, i_state)})
        import pdb; pdb.set_trace()


    def smooth(self, obj_func, bounds=None):
        constraints = []
        if self.dyn_func is not None:
            for i_step in range(len(self.initial_states)-1):
                for i_state in range(self.num_states):
                    constraints.append({'type': 'eq', 'fun': lambda x: self.get_non_linear_constraint(x, i_step, i_state)})

        for i_state in range(self.num_states):
            constraints.append({'type': 'eq', 'fun': lambda x: self.get_boundary_constraint(x, self.start_state, 0, i_state)})
            constraints.append({'type': 'eq', 'fun': lambda x: self.get_boundary_constraint(x, self.end_state, -1, i_state)})

        initial_guess = self.pack_decision_vars(self.initial_states, self.initial_controls)
        solution = scipy.optimize.minimize(obj_func, initial_guess, method='SLSQP', bounds=bounds, constraints=constraints)
        if not solution.success:
            print(solution)
            pdb.set_trace()
            raise SmootherException('Failed to find solution from optimization.')
        return solution.x

    def pack_decision_vars(self, states, controls):
        state_controls = np.hstack((states, controls))
        return np.reshape(state_controls, (-1, 1), order='F').flatten()

    def unpack_decision_vars(self, decision_vars):
        stacked = np.reshape(decision_vars, (-1, self.num_states + self.num_control_states), order='F')
        return stacked[:, :self.num_states], stacked[:, self.num_states:]

    def get_non_linear_constraint(self, decision_vars, i_step, i_state):
        states, control = self.unpack_decision_vars(decision_vars)
        est_next_state = self.dyn_func(states[i_step], control[i_step])
        return est_next_state[i_state] - states[i_step+1][i_state]

    def get_boundary_constraint(self, decision_vars, state, i_step, i_state):
        states, control = self.unpack_decision_vars(decision_vars)
        if not isinstance(state, np.ndarray):
            state = np.expand_dims(np.array(state), axis=0)
        print(states[i_step, i_state], state[i_state])
        return states[i_step, i_state] - state[i_state]

    def rk4(self, dyn_func, t_prev, t_next, z_prev):
        # type: (Callable[[float, Sequence[float]], Sequence[float]], float, float, Sequence[float]) -> Tuple[Sequence[float], int]
        h = t_next - t_prev
        t_mid = t_prev + 0.5 * h
        k1 = h* dyn_func(t_prev, z_prev)
        k2 = h* dyn_func(t_mid, z_prev + 0.5 * k1)
        k3 = h* dyn_func(t_mid, z_prev + 0.5 * k2)
        k4 = h * dyn_func(t_next, z_prev + k3)
        z_del = (1.0/6.0) * (k1 + 2*k2 + 2*k3 + k4)
        z_next = z_prev + z_del
        return z_next, 4  # 4 dynamics function evalutations





        return zNext, nEval

