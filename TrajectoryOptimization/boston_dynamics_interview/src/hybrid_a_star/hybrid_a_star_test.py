import numpy as np
import pytest

# import pdb
# import matplotlib.pyplot as plt
from src.hybrid_a_star import hybrid_a_star


@pytest.fixture()
def map_with_no_obstacles():
    return np.zeros((10, 10))


@pytest.fixture()
def n1():
    return hybrid_a_star.Node(10, 0, (0.0, 0.0, 0.0), (0.1, 0.1, 1.0), 0.1, 0.1)


@pytest.fixture
def n2():
    return hybrid_a_star.Node(20, 0, (1.0, 0.0, 0.0), (1.1, 0.1, 1.0), 0.1, 0.1)


def test_node(n1, n2):
    assert n1 < n2


def test_priority_queue(n1, n2):
    node_list = hybrid_a_star.NodeList()
    node_list.add(n1)
    node_list.add(n2)
    assert n2 == node_list.get_lowest_cost()

def test_find_node(n1, n2):
    node_queue = hybrid_a_star.NodeList()
    node_queue.add(n1)
    assert node_queue.find_node(n1.discrete_pos)
    assert node_queue.find_node(n2) is None


# def test_path_when_no_obstacles(map_with_no_obstacles):
#     hybrid_a_star.find_path(start, goal, obstacle_checker, heuristic)

