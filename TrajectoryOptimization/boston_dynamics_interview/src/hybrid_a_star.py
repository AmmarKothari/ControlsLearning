import numpy as np
import heapq
import math
import functools

class Node:
    def __init__(self, g, c, discrete_pos, pos, turning_radius, distance, parent_node=None):
        # Estimated cost to go
        self.g = g

        # Actual cost to this node
        self.c = c

        self.discrete_pos = discrete_pos
        self.pos = pos
        self.turning_radius = turning_radius
        self.distance = distance

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

    def find_node(self, match_discrete_pos):
        stop_search_at = -1
        for i, node in enumerate(self.list):
            if self._is_node_discrete_pos_equal(node, match_discrete_pos):
                return self.list.pop(i)
        return None

    def _is_node_discrete_pos_equal(self, node1, match_discrete_pos):
        for v, z in zip(node1.discrete_pos, match_discrete_pos):
            if v != z:
                return False
        return True


class RSModel:
    def step(self, start, turning_radius, drive_distance):
        theta = self._calc_theta(turning_radius, drive_distance)
        x_new = start[0] + turning_radius * math.cos(start[2])
        y_new = start[1] + turning_radius * math.sin(start[2])
        yaw_new = start[2] + theta
        return x_new, y_new, yaw_new

    @functools.lru_cache()
    def _calc_theta(self, turning_radius, drive_distance):
        return drive_distance / turning_radius * 2 * math.pi




class HybridAStarConfig:
    def __init__(self, model, heuristic, map, max_turning_radius, max_drive_distance):
        self.model = model
        self.heuristic = heuristic
        self.map = map
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
        priority_queue = []
        closed_set = {}

        start_node = self.get_start_node(start, goal)

        heapq.heappush(priority_queue, start_node)
        new_nodes = self.expand_node(start_node)

    def get_start_node(self, start, goal):
        return Node(0, self.config.heuristic.cost(start, goal), self.config.map.to_map_pos(start), start, 0.0, 0.0)

    def expand_node(self, node):
        for turning_radius in self.turning_radii:
            for drive_distance in self.drive_distances:
                next_pt = self.config.model.step(node.pos, turning_radius, drive_distance)


