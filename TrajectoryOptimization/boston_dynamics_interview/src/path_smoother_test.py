import math
import pdb

import matplotlib.pyplot as plt
import numpy as np
import pytest
from src import path_smoother, rs_model


# ~~~~~~~~~~~~~~~~~~~
# Fixtures
# ~~~~~~~~~~~~~~~~~~~
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
    input_vars = np.hstack((np.zeros((10,1)), np.full((10,1),1)))
    ps = path_smoother.PathSmoother(2, None)
    packed_vars = ps.pack_decision_vars(input_vars)
    assert all(list(v == 0.0 for v in packed_vars[:10]))
    assert all(list(v == 1.0 for v in packed_vars[10:]))


def test_unpack():
    input_vars = np.hstack((np.zeros((10,1)), np.full((10,1),1)))
    ps = path_smoother.PathSmoother(2, None)
    assert np.array_equal(input_vars, ps.unpack_decision_vars(ps.pack_decision_vars(input_vars)))


def test_smooth_for_simple_obj_fun():
    dyn_func = lambda x: x
    obj_func = lambda x: sum(np.square(np.array(x)))
    ps = path_smoother.PathSmoother(1, dyn_func)
    smoothed_vals = ps.smooth(np.arange(0, 10), obj_func, None)
    assert np.array_equal(smoothed_vals, np.full(10, -1))



def test_smooth_straight_path_when_no_obstacles(config_with_no_obstacles):
    ps = path_smoother.PathSmoother()
    obstacle_map, start, control_path, model = config_with_no_obstacles
    pos_path = model.get_path_from_control(start, control_path)
    _plot_path(pos_path)
    ps.smooth(control_path, obstacle_map, model)



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
