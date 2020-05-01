import numpy as np
import pytest

from src.hybrid_a_star import hybrid_a_star, motion_models, heuristic, \
    obstacle_check as obstacle_check_module, nav_map


@pytest.fixture
def four_direction():
    return motion_models.FourDirection()


@pytest.fixture
def eight_direction():
    return motion_models.EightDirection()


@pytest.fixture
def simple_map():
    return nav_map.NavMap(np.zeros((10, 10)).astype(bool), resolution=0.1)


@pytest.fixture
def obstacle_check(simple_map):
    return obstacle_check_module.ObstacleCheck(simple_map)


@pytest.fixture
def four_direction_config(four_direction, obstacle_check):
    return hybrid_a_star.HybridAStarConfig(four_direction, heuristic.simple_heuristic, obstacle_check, 1.1, 0)


@pytest.fixture
def eight_direction_config(eight_direction, obstacle_check):
    return hybrid_a_star.HybridAStarConfig(eight_direction, heuristic.simple_heuristic, obstacle_check, 1.1, 0)


def test_simple_plan_four(four_direction_config, simple_map):
    start_map_pose = [0, 0, 0]
    end_map_pose = [10, 10, 0]
    path_finder = hybrid_a_star.HybridAStar(four_direction_config)
    start_discrete = obstacle_check_module.to_continuous_obstacle_map_pos(start_map_pose, simple_map)
    end_discrete = obstacle_check_module.to_continuous_obstacle_map_pos(end_map_pose, simple_map)
    path = path_finder.find_path(start_discrete, end_discrete)
    assert len(path) > 0
    np.testing.assert_array_equal(path[0], start_map_pose)
    np.testing.assert_array_equal(path[-1], end_map_pose)


def test_simple_plan_eight(eight_direction_config, simple_map):
    start_map_pose = [0, 0, 0]
    end_map_pose = [10, 10, 0]
    path_finder = hybrid_a_star.HybridAStar(eight_direction_config)
    start_discrete = obstacle_check_module.to_continuous_obstacle_map_pos(start_map_pose, simple_map)
    end_discrete = obstacle_check_module.to_continuous_obstacle_map_pos(end_map_pose, simple_map)
    path = path_finder.find_path(start_discrete, end_discrete)
    _plot_path(path)
    assert len(path) > 0
    np.testing.assert_array_equal(path[0], start_map_pose)
    np.testing.assert_array_equal(path[-1], end_map_pose)


def _plot_path(path):
    import matplotlib.pyplot as plt
    xs = [p[0] for p in path]
    ys = [p[1] for p in path]
    plt.plot(xs, ys, 'rx-')
    plt.show()
