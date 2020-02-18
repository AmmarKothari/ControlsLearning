import numpy as np


NUM_STATES = 6

class Params:
    def __init__(self):
        self.wind = np.array([2.0, 4.0, 0.0])
        self.mass = 1.0
        self.gravity = 9.8
        self.start = [0.0, 0.0, 0.0]
        self.target = [10.0, 0.0, 10.0]


def get_end_state_defect(dec_vars, params, i_state):
    total_time, initial_vel = unpack_dec_vars(dec_vars)
    end_state = run_simulation(total_time, initial_vel, params)
    return params.target[i_state] - end_state[i_state]


def get_start_state_defect():
    pass


def run_simulation(total_time, initial_vel, params):
    initial_state = np.hstack((params.start, initial_vel))
    dt = 0.1
    states = [initial_state]
    for _ in np.arange(0, total_time, dt):
        dz = dynamics(states[-1], params)
        next_state = states[-1] + dt * dz
        states.append(next_state)
    return states[-1]




class Guess:
    def __init__(self):
        self.total_time = 0.0
        self.initial_vel = [0.0, 0.0, 0.0]



def dynamics(state, params):
    force_gravity = np.array([0.0, 0.0, -params.mass * params.gravity])
    accel = force_gravity / params.mass
    dz = np.hstack((state[3:], accel))
    return dz


def pack_dec_vars(total_time, initial_vel):
    dec_vars = [total_time]
    for v in initial_vel:
        dec_vars.append(v)
    return dec_vars


def unpack_dec_vars(dec_vars):
    return dec_vars[0], dec_vars[1:]



