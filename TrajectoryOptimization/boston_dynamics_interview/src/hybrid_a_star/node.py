import functools
import heapq


class Node:
    def __init__(self, g, c, discrete_pos, pos, turning_radius, distance, parent_node=None):
        # Estimated cost to go
        self.g = g

        # Actual cost to this node
        self.c = c

        # Discrete position on search grid
        self.discrete_pos = discrete_pos

        # Continuous position
        self.pos = pos

        # Previous node to find the path home
        self.parent_node = parent_node

        # Values to rebuild control path
        # Distance driven
        self.distance = distance

        # Turning radius
        self.turning_radius = turning_radius

    @property
    def total_cost(self):
        return self.g + self.c

    def __hash__(self):
        return hash('{}{}{}'.format(*self.discrete_pos))

    def __lt__(self, other):
        return self.total_cost < other.total_cost

    def __gt__(self, other):
        return not self < other

    def __eq__(self, other):
        return self.total_cost == other.total_cost

    def __repr__(self):
        return '{}, Cost: {}'.format(self.pos, self.total_cost)

    def __str__(self):
        return self.__repr__()


class NodeHeap:
    def __init__(self):
        self.list = []

    def add(self, node):
        heapq.heappush(self.list, node)

    def pop_lowest_cost_node(self):
        # Nodes should be sorted by total cost on heap
        return self.list.pop(0)

    def get_lowest_cost(self):
        return min(self.list, key=lambda x: x.total_cost)

    @functools.singledispatch
    def get_node(self, match_discrete_pos):
        for i, node in enumerate(self.list):
            if _is_node_discrete_pos_equal(node, match_discrete_pos):
                return self.list.pop(i)
        return None

    @get_node.register(Node)
    def _(self, match_node):
        return self.get_node(match_node.discrete_pos)

    def has_node(self, match_node):
        for i, node in enumerate(self.list):
            if _is_node_discrete_pos_equal(node, match_node.discrete_pos):
                return True
        return False

    def __len__(self):
        return len(self.list)


class NodeList:
    def __init__(self):
        self.list = []

    def add(self, node):
        heapq.heappush(self.list, hash(node))

    def is_in_list(self, node):
        return hash(node) in self.list


def _is_node_discrete_pos_equal(node1, match_discrete_pos):
    for v, z in zip(node1.discrete_pos, match_discrete_pos):
        if v != z:
            return False
    return True
