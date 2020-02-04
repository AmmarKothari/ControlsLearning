from typing import Callable
from typing import Sequence
from typing import Tuple

import numpy as np
import scipy.optimize


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

    def smooth(self, initial_control_path, obj_func, constraint_func):
        import pdb
        pdb.set_trace()
        scipy.optimize.minimize(obj_func, initial_control_path)

    def pack_decision_vars(self, control_path):
        return np.reshape(control_path, (-1, 1), order='F').flatten()

    def unpack_decision_vars(self, decision_vars):
        return np.reshape(decision_vars, (-1, self.num_states), order='F')


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

