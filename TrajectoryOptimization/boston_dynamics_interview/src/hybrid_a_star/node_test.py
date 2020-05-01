import numpy as np
import pytest

from src.hybrid_a_star import node


@pytest.fixture()
def map_with_no_obstacles():
    return np.zeros((10, 10))


@pytest.fixture()
def n1():
    return node.Node(10, 0, (0.0, 0.0, 0.0), (0.1, 0.1, 1.0), 0.1, 0.1)


@pytest.fixture
def n2(n1):
    return node.Node(20, 0, (1.0, 0.0, 0.0), (1.1, 0.1, 1.0), 0.1, 0.1, n1)


@pytest.fixture
def node_list():
    return node.NodeList()


def test_node_comparison(n1, n2):
    assert n1 < n2


def test_priority_queue(n1, n2):
    node_list = node.NodeHeap()
    node_list.add(n1)
    node_list.add(n2)
    assert n1 == node_list.get_lowest_cost()


def test_find_node(n1, n2):
    node_queue = node.NodeHeap()
    node_queue.add(n1)
    assert node_queue.get_node(n1.discrete_pos) == n1
    assert node_queue.get_node(n2) is None


def test_node_list_add(n1, n2, node_list):
    node_list.add(n1)
    assert node_list.is_in_list(n1)
    assert not node_list.is_in_list(n2)


def test_pop_lowest_cost_node(n1, n2):
    node_list = node.NodeHeap()
    node_list.add(n1)
    node_list.add(n2)
    assert n1 == node_list.pop_lowest_cost_node()
