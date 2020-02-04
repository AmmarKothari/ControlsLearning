import functools

import pytest
import pdb
from src import double_integrator_model, path_smoother
import numpy as np

import scipy.optimize

@pytest.fixture
def di():
    return double_integrator_model.DoubleIntegrator()


@pytest.fixture
def ps_di(di):
    return path_smoother.PathSmoother(di.num_states, di.num_control_states, di.step)


class TrajectoryConfig:
    def __init__(self, nStep, begin_state, final_state, duration):
        self.nStep = nStep
        self.begin_state = begin_state
        self.final_state = final_state
        self.duration = duration

class OptimizerConfig:
    def __init__(self, objective_func):
        pass


def object_func(dec_variables, ps_config):
    state, control = unpack(dec_variables, ps_config)
    return sum(path_integral(control) * ps_config.h_step)


def path_integral(controls):
    return sum([sum([x**2 for x in control]) for control in controls])


def pack(states, control):
    return np.hstack((np.hstack(states), np.hstack(control))).reshape(-1, 1)


def unpack(dec_vars, ps_config):
    state = np.reshape(dec_vars[:], (-1, ps_config.n_states), order='F')
    control = np.reshape(dec_vars[(ps_config.n_grid * ps_config.n_states):], (-1, ps_config.n_control), order='F')
    return state, control


def get_defect(dec_vars, n_step, ps_config):
    states, controls = unpack(dec_vars, ps_config)
    start_state = states[n_step]
    end_state = states[n_step + 1]
    control = controls[n_step]
    # Only works if its a dynamic step, but if its a length based step, this needs to be hard coded to a value
    # like the assumed dt in the RS model
    est_end_state = ps_config.model.step(start_state, control, ps_config.h_step)
    defect = est_end_state - end_state
    return defect


def test_ps_di(ps_di, di):
    start = [0.0, 0.0, 0.0, 0.0]
    goal = [10.0, 0.0, 0.0, 0.0]

    traj_config = TrajectoryConfig(10, start, goal, 10)
    ps_config = path_smoother.PathSmootherConfig(
        n_states = di.num_states,
        n_control = di.num_control_states,
        t_boundary = [0.0, traj_config.duration],
        state_boundary= [traj_config.begin_state, traj_config.final_state],
        n_grid = traj_config.nStep + 1,
        t_grid = np.linspace(0, traj_config.duration, traj_config.nStep + 1),
        h_step = np.diff([0.0, traj_config.duration])[0] / traj_config.nStep,
        model = di
    )

    # Setup objective func
    obj_func = functools.partial(object_func, ps_config=ps_config)
    opt_config = OptimizerConfig(obj_func)

    # Generate initial guess
    xGrid = [np.linspace(0.0, 10.0, ps_config.n_grid), np.zeros(ps_config.n_grid), np.zeros(ps_config.n_grid), np.zeros(ps_config.n_grid)]
    uGrid = [np.linspace(0.0, 0.0, ps_config.n_grid-1), np.linspace(0.0, 0.0, ps_config.n_grid-1)]  # only control between states (not before start state) so one less control state
    dec_vars_0 = pack(xGrid, uGrid)

    # No inequality constraints
    Aineq = []
    Bineq = []

    # Set boundary conditions
    eq_constraints = set_boundary_conditions(ps_config)

    # Set knot point (non-linear) constraints - Woooo!
    dynamic_eq_constraints = set_dynamic_constraints(ps_config)

    all_constraints = eq_constraints + dynamic_eq_constraints
    all_constraints[-1]['fun'](dec_vars_0)

    # in practice, we could add some inequality constraints to the actual control inputs

    scipy.optimize.minimize(obj_func, dec_vars_0, method='SLSQP', constraints=all_constraints)
    pdb.set_trace()




def set_dynamic_constraints(ps_config):

    """
    1. unpack the vars
    2. grab the right variable
    3. step through dynamics
    4. calculate defect
    """

    dynamic_eq_constraints = []
    for i_step in range(ps_config.n_grid-1):
        constraint = {'type': 'eq', 'fun': lambda x: get_defect(x, i_step, ps_config)}
        dynamic_eq_constraints.append(constraint)
    return dynamic_eq_constraints


def set_boundary_conditions(ps_config):
    # This is hacky, but i think the right idea.  How to generalize?
    # This does assume an implicit ordering of the vars.  Better to rely on pack and unpack
    eq_constraints = [
    # Start constraints
    {'type': 'eq', 'fun': lambda x: x[0*ps_config.n_grid] - ps_config.state_boundary[0][0]},
    {'type': 'eq', 'fun': lambda x: x[1*ps_config.n_grid] - ps_config.state_boundary[0][1]},
    {'type': 'eq', 'fun': lambda x: x[2*ps_config.n_grid] - ps_config.state_boundary[0][2]},
    {'type': 'eq', 'fun': lambda x: x[3*ps_config.n_grid] - ps_config.state_boundary[0][3]},

    # End constraints
    {'type': 'eq', 'fun': lambda x: x[1 * ps_config.n_grid - 1] - ps_config.state_boundary[1][0]},
    {'type': 'eq', 'fun': lambda x: x[2 * ps_config.n_grid - 1] - ps_config.state_boundary[1][1]},
    {'type': 'eq', 'fun': lambda x: x[3 * ps_config.n_grid - 1] - ps_config.state_boundary[1][2]},
    {'type': 'eq', 'fun': lambda x: x[4 * ps_config.n_grid - 1] - ps_config.state_boundary[1][3]}
    ]
    return eq_constraints
