import math
import pdb

import matplotlib.pyplot as plt
import numpy as np
import pytest
import scipy.optimize
from src import path_smoother, rs_model


# ~~~~~~~~~~~~~~~~~~~
# Fixtures
# ~~~~~~~~~~~~~~~~~~~
from src.models import projectile


@pytest.fixture()
def config_with_no_obstacles():
    map_size = 10
    drive_distance = 1.0
    turn_amount = 0.0
    start = [0.0, 0.0, 0.0]
    control_path = [[(-1)**i*turn_amount, drive_distance] for i in range(10)]
    model = rs_model.RSModel()
    return np.zeros((map_size, map_size)), start, control_path, model


# ~~~~~~~~~~~~~~~~~~~
# Helper Tests
# ~~~~~~~~~~~~~~~~~~~
def test_pack():
    states = np.zeros((10, 1))
    controls = np.ones((10, 1))
    ps = path_smoother.PathSmoother(1, 1, None)
    packed_vars = ps.pack_decision_vars(states, controls)
    assert all(list(v == 0.0 for v in packed_vars[:10]))
    assert all(list(v == 1.0 for v in packed_vars[10:]))


def test_unpack():
    states = np.zeros((10, 1))
    controls = np.ones((10, 1))
    ps = path_smoother.PathSmoother(1, 1, None)
    states_out, controls_out = ps.unpack_decision_vars(ps.pack_decision_vars(states, controls))
    np.testing.assert_array_equal(states, states_out)
    np.testing.assert_array_equal(controls, controls_out)


def test_non_linear_constraint():
    states = np.zeros((10, 1))
    controls = np.ones((10, 1))
    dyn_func = lambda x, y: x + 1
    ps = path_smoother.PathSmoother(1, 1, dyn_func)
    dec_vars = ps.pack_decision_vars(states, controls)
    for i_step in range(len(states)-1):
        defect = ps.get_non_linear_constraint(dec_vars, i_step, 0)
        assert defect == 1





def test_smooth_for_simple_obj_fun():
    def dyn_func(x, y):
        return x+y

    def obj_func(x):
        cost = sum(np.square(np.array(x)[-10:]))
        print(cost)
        return cost

    initial_state_guess = np.arange(0, 10)
    initial_control_guess = np.zeros(10)

    ps = path_smoother.PathSmoother(1, 1, dyn_func)
    ps.set_initial_guess(initial_state_guess, initial_control_guess)
    ps.set_boundary_states(initial_state_guess[0], initial_state_guess[-1])

    smoothed_vals = ps.smooth(obj_func)
    assert np.isclose(obj_func(smoothed_vals), 0.0)

# def test_smooth_for_simple_obj_fun_with_bounds():
#     dyn_func = None
#     obj_func = lambda x: sum(np.square(np.array(x)))
#     initial_guess = np.arange(0, 10)
#     bounds = []
#     for _ in initial_guess:
#         bounds.append([1.0, 2.0])
#     ps = path_smoother.PathSmoother(1, 0, dyn_func)
#     smoothed_vals = ps.smooth(initial_guess, obj_func, bounds=bounds)
#     assert np.isclose(obj_func(smoothed_vals), 10.0)
#
#
# def test_smooth_for_simple_obj_fun_with_dynamic_constraints():
#     dyn_func = lambda x: x + 1
#     obj_func = lambda x: sum(np.square(np.array(x)))
#     ps = path_smoother.PathSmoother(1, 0, dyn_func)
#     smoothed_vals = ps.smooth(np.arange(0, 10), obj_func)
#     assert np.isclose(obj_func(smoothed_vals), 0.0)


# def test_smooth_straight_path_when_no_obstacles(config_with_no_obstacles):
#     ps = path_smoother.PathSmoother()
#     obstacle_map, start, control_path, model = config_with_no_obstacles
#     pos_path = model.get_path_from_control(start, control_path)
#     _plot_path(pos_path)
#     ps.smooth(control_path, obstacle_map, model)



def _plot_path(path):
    path = np.array(path)
    i = 0
    for wp in path:
        dx, dy = 0.1*math.cos(wp[2]), 0.1*math.sin(wp[2])
        plt.arrow(wp[0], wp[1], dx, dy)
        plt.text(wp[0], wp[1], str(i))
        i += 1
    plt.plot(path[1:-1, 0], path[1:-1, 1], 'rx')
    plt.plot(path[0, 0], path[0, 1], 'bo')
    plt.plot(path[-1, 0], path[-1, 1], 'gs')
    plt.axis('equal')
    plt.show()
