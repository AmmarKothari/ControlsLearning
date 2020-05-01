import math

import numpy as np
import pytest

from src.hybrid_a_star import obstacle_check
from src.hybrid_a_star import nav_map


@pytest.fixture
def simple_map():
    simple_map = np.zeros((10, 10)).astype(bool)
    simple_map[0, 0] = True
    return nav_map.NavMap(simple_map, resolution=0.1, min_xy=(0, 0))


@pytest.fixture
def simple_map_offset():
    simple_map = np.zeros((20, 20)).astype(bool)
    simple_map[0, 0] = True
    return nav_map.NavMap(simple_map, resolution=0.1, min_xy=(-1, -1))


@pytest.fixture
def image_path():
    return '/Users/ammarkothari/Projects/ControlsLearning/TrajectoryOptimization/boston_dynamics_interview/src/maze-png/maze512-1-0.png'


@pytest.fixture
def simple_check(simple_map):
    simple_check = obstacle_check.ObstacleCheck(simple_map)
    return simple_check


@pytest.fixture
def complex_check(image_path):
    complex_check = obstacle_check.from_image_path(image_path)
    return complex_check


def test_simple_check(simple_check):
    assert simple_check.check((0, 0, 0))
    assert simple_check.check((0, 0, 1))
    assert not simple_check.check((1, 1, 0))


def test_complex_check(complex_check):
    # for i in range(100):
    #     for j in range(100):
    #         collision = complex_check.check((i, j, 0))
    #         if collision:
    #             print('x: {}, y:{}'.format(i, j))
    #             return
    # import pdb; pdb.set_trace()
    assert not complex_check.check((0, 0, 0))
    assert complex_check.check((4, 4, 0))


def test_to_discrete_obstacle_map_pose(simple_map):
    map_position = [0.05, 0.05, math.pi / 4]
    continuous_obstacle_map_position = obstacle_check.to_continuous_obstacle_map_pos(map_position, simple_map)
    discrete_map_position = obstacle_check.to_discrete_obstacle_map_pos(continuous_obstacle_map_position)
    assert discrete_map_position[0] == 0 and discrete_map_position[1] == 0
    assert discrete_map_position[2] == 5
    map_position = [-0.05, -0.05, 0]
    with pytest.raises(Exception):
        obstacle_check.to_continuous_obstacle_map_pos(map_position, simple_map)

    map_position = [0.09, 0.08, 0]
    continuous_obstacle_map_position = obstacle_check.to_continuous_obstacle_map_pos(map_position, simple_map)
    discrete_map_position = obstacle_check.to_discrete_obstacle_map_pos(continuous_obstacle_map_position)
    assert discrete_map_position[0] == 1 and discrete_map_position[1] == 1


def test_to_discrete_obstacle_map_pose_offset_map(simple_map_offset):
    map_position = [-0.9, -0.8, 0]
    continuous_obstacle_map_position = obstacle_check.to_continuous_obstacle_map_pos(map_position, simple_map_offset)
    discrete_map_position = obstacle_check.to_discrete_obstacle_map_pos(continuous_obstacle_map_position)
    assert discrete_map_position[0] == 1 and discrete_map_position[1] == 2

    map_position = [-1.5, 0.0, 0]
    with pytest.raises(Exception):
        obstacle_check.to_continuous_obstacle_map_pos(map_position, simple_map_offset)
