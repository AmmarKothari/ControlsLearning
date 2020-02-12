import numpy as np


def pack(states, control):
    return np.hstack((np.hstack(states), np.hstack(control))).reshape(-1, 1)


def unpack(dec_vars, ps_config):
    state = np.reshape(dec_vars[:], (-1, ps_config.n_states), order='F')
    control = np.reshape(dec_vars[(ps_config.n_grid * ps_config.n_states):], (-1, ps_config.n_control), order='F')
    return state, control