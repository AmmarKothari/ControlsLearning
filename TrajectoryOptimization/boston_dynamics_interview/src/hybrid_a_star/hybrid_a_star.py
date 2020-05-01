import math

import numpy as np

from src.hybrid_a_star import node, obstacle_check as obstacle_check_module

import pdb


class HybridAStarConfig:
    def __init__(self, model, heuristic, obstacle_check, max_turning_radius, max_drive_distance):
        self.model = model
        self.heuristic = heuristic
        self.obstacle_check = obstacle_check
        self.max_turning_radius = max_turning_radius
        # self.turning_radius_steps = turning_radius_steps
        self.max_drive_distance = max_drive_distance
        # self.drive_distance_steps =
        # self.cost_func = cost_func
        self.grid_size = 1.0


class HybridAStar:
    def __init__(self, config):
        self.config = config
        self.turning_radii = np.linspace(-self.config.max_turning_radius, self.config.max_turning_radius, 3)
        self.drive_distances = [-self.config.max_drive_distance, self.config.max_drive_distance]
        self.open_nodes = node.NodeHeap()
        self.closed_nodes = node.NodeList()

    def find_path(self, start, goal):
        # start and end should be in continuous obstacle map coordinates
        closed_set = {}
        path = []

        start_node = self.get_start_node(start, goal)
        self.open_nodes.add(start_node)
        nodes_expanded = 1
        close_to_goal = False
        path = []
        while len(self.open_nodes) and not close_to_goal:
            lowest_cost_node = self.open_nodes.pop_lowest_cost_node()
            print('Current lowest cost: {:.3f}'.format(lowest_cost_node.total_cost))
            neighbor_poses = self.expand_node(lowest_cost_node)
            for neighbor_pose, distance, turning_radius in neighbor_poses:
                neighbor_node = self.open_nodes.get_node(neighbor_pose)
                # Because nodes that have already been expanded are not re-expanded,
                # will result in potentially suboptimal path.
                if self.closed_nodes.is_in_list(neighbor_node):
                    continue
                # TODO: Change this with a cost function that is part of the config
                transition = abs(distance * turning_radius)
                if neighbor_node:
                    # check if cost to reach is lower than current cost to reach of node.
                    if neighbor_node.g > (lowest_cost_node.g + transition):
                        # Update node with new info
                        neighbor_node.g = lowest_cost_node.g + transition
                        neighbor_node.parent_node = lowest_cost_node
                        neighbor_node.pos = neighbor_pose
                    self.open_nodes.add(neighbor_node)
                else:
                    new_node = self._add_new_node(distance, goal, lowest_cost_node, neighbor_pose, transition,
                                                  turning_radius)
                    self.open_nodes.add(new_node)
                    # if within goal region, then stop search
                    if math.hypot(new_node.pos[0] - goal[0], new_node.pos[1] - goal[1]) < 2.0:
                        close_to_goal = True
                        path = self.build_path(new_node, goal)

            nodes_expanded += 1
            if nodes_expanded % 1000 == 0:
                print('Total nodes expanded: {}'.format(nodes_expanded))
            self.closed_nodes.add(lowest_cost_node)
        print('Total nodes expanded: {}'.format(nodes_expanded))
        print('Total path length: {}'.format(len(path)))
        map_path = []
        for waypoint in path:
            map_path.append(obstacle_check_module.from_continuous_obstacle_map_pos(waypoint,
                                                                                   self.config.obstacle_check.obstacle_map))
        return map_path

    def build_path(self, new_node, goal):
        current_node = new_node
        path = []
        while True:
            if current_node is None:
                path.reverse()
                break
            path.append(current_node.pos)
            current_node = current_node.parent_node
        path.append(goal)
        return path

    def _add_new_node(self, distance, goal, lowest_cost_node, neighbor_pose, transition, turning_radius):
        neighbor_discrete_pos = obstacle_check_module.to_discrete_obstacle_map_pos(neighbor_pose)
        new_node = node.Node(g=lowest_cost_node.g + transition,
                             c=self.config.heuristic(neighbor_pose, goal),
                             discrete_pos=neighbor_discrete_pos,
                             pos=neighbor_pose,
                             parent_node=lowest_cost_node,
                             distance=distance,
                             turning_radius=turning_radius)
        return new_node

    def get_start_node(self, start, goal):
        discrete_pos = obstacle_check_module.to_discrete_obstacle_map_pos(start)
        start_node = node.Node(g=0,
                               c=self.config.heuristic(start, goal),
                               discrete_pos=discrete_pos,
                               pos=start,
                               parent_node=None,
                               distance=0,
                               turning_radius=0)
        return start_node

    def expand_node(self, node):
        for turning_radius in self.turning_radii:
            for drive_distance in self.drive_distances:
                neighbor_poses = self.config.model.step(node.pos, turning_radius, drive_distance)
                for neighbor_pose in neighbor_poses:
                    yield neighbor_pose, drive_distance, turning_radius
