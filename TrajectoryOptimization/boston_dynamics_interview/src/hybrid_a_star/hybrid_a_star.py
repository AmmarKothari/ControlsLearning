import numpy as np
import heapq
import math
import functools

import pdb


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

    def get_total_cost(self):
        return self.g + self.c

    def __lt__(self, other):
        return self.get_total_cost() < other.get_total_cost()

    def __gt__(self, other):
        return not self < other

    def __eq__(self, other):
        return self.get_total_cost() == other.get_total_cost()


class NodeList:
    def __init__(self):
        self.list = []

    def add(self, node):
        heapq.heappush(self.list, node)

    def get_lowest_cost(self):
        return heapq.nlargest(1, self.list)[0]

    @functools.singledispatch
    def find_node(self, match_discrete_pos):
        for i, node in enumerate(self.list):
            if self._is_node_discrete_pos_equal(node, match_discrete_pos):
                return self.list.pop(i)
        return None

    @find_node.register(Node)
    def _(self, match_node):
        self.find_node(match_node.discrete_pos)

    def _is_node_discrete_pos_equal(self, node1, match_discrete_pos):
        for v, z in zip(node1.discrete_pos, match_discrete_pos):
            if v != z:
                return False
        return True

    def __len__(self):
        return len(self.list)




class HybridAStarConfig:
    def __init__(self, model, heuristic, obstacle_map, max_turning_radius, max_drive_distance):
        self.model = model
        self.heuristic = heuristic
        self.obstacle_map = obstacle_map
        self.max_turning_radius = max_turning_radius
        # self.turning_radius_steps = turning_radius_steps
        self.max_drive_distance = max_drive_distance
        # self.drive_distance_steps =

        self.grid_size = 1.0


class HybridAStar():
    def __init__(self, config):
        self.config = config
        self.turning_radii = np.linspace(-self.config.max_turning_radius, self.config.max_turning_radius, 3)
        self.drive_distances = [-self.config.max_drive_distance, self.config.max_drive_distance]
        # self.motion_primitives = self.get_motion_primitives()


    def find_path(self, start, goal, obstacle_checker):
        priority_queue = NodeList()
        closed_set = {}
        path = []

        start_node = self.get_start_node(start, goal)
        priority_queue.add(start_node)

        while len(priority_queue):
            lowest_cost_node = priority_queue.get_lowest_cost()
            for new_pos in self.expand_node(lowest_cost_node):
                node_for_pos = priority_queue.find_node(new_pos)
                if node_for_pos is None:
                    pass
                    # Add node
                    # if within goal region, then stop search
                else:
                    pass
                    # check if new cost is lower than current cost
                        # if yes, update node with new info
                        # else, do nothing.
            # add just expanded node to the closed set
        # reconstruct path
        return path

    def get_start_node(self, start, goal):
        return Node(0, self.config.heuristic.cost(start, goal), self.config.map.to_map_pos(start), start, 0.0, 0.0)

    def expand_node(self, node):
        for turning_radius in self.turning_radii:
            for drive_distance in self.drive_distances:
                yield self.config.model.step(node.pos, turning_radius, drive_distance)

