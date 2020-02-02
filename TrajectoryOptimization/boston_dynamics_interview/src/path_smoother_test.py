import math

import pdb

import pytest
import numpy as np
import matplotlib.pyplot as plt

from src import path_smoother, hybrid_a_star


@pytest.fixture()
def map_with_no_obstacles():
    map_size = 10
    drive_distance = 1.0
    turn_amount = 1.0
    start = [0.0, 0.0, 0.0]
    pos_path = [start]
    control_path = [[-1**i*turn_amount, drive_distance] for i in range(10)]
    model = hybrid_a_star.RSModel()

    # pos_path = model.get_path_from_control(start, control_path)

    # new_pos = pos_path[-1]
    # for i in range(map_size):
    #     new_pos = model.step(new_pos, turn_value, drive_distance)
    #     pos_path.append(new_pos)
    #     control_path.append([turn_value, drive_distance])
    return np.zeros((map_size, map_size)), start, control_path, model


def test_smooth_straight_path_when_no_obstacles(map_with_no_obstacles):
    ps = path_smoother.PathSmoother()
    obstacle_map, start, control_path, model = map_with_no_obstacles
    pos_path = model.get_path_from_control(start, control_path)
    _plot_path(pos_path)
    plt.show()
    # pdb.set_trace()
    # ps.smooth(path, obstacle_map, model)



def _plot_path(path):
    path = np.array(path)
    for wp in path:
        dx, dy = 0.1*math.sin(wp[2]), 0.1*math.cos(wp[2])
        # plt.arrow(wp[0], wp[1], dx, dy)
    plt.plot(path[1:-1, 0], path[1:-1, 1], 'rx')
    plt.plot(path[0, 0], path[0, 1], 'bo')
    plt.plot(path[-1, 0], path[-1, 1], 'gs')