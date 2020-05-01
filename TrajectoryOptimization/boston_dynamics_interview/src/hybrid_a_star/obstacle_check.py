import functools
import math

import PIL.Image as PImage
import numpy as np


class ObstacleCheck:
    def __init__(self, obstacle_map):
        # Array with cells marked as True if they have an obstacle in it.
        self.obstacle_map = obstacle_map

    def check(self, discrete_position):
        # Position in obstacle map coordinates
        return self.obstacle_map[discrete_position[0], discrete_position[1]]


def to_continuous_obstacle_map_pos(map_position, navmap):
    x = (map_position[0] - navmap.min_xy[0]) / navmap.resolution
    y = (map_position[1] - navmap.min_xy[1]) / navmap.resolution
    yaw = map_position[2] / navmap.yaw_resolution
    # Should be in the 0 - 2pi region
    if yaw < 0:
        yaw += math.pi * 2 / navmap.yaw_resolution
    if not (navmap.min_idxs[0] <= x <= navmap.max_idxs[0]):
        raise Exception('X index out of map bounds')
    if not (navmap.min_idxs[1] <= y <= navmap.max_idxs[1]):
        raise Exception('Y index out of map bounds')

    return [x, y, yaw]


def to_discrete_obstacle_map_pos(map_position):
    """Given a pose in continuous obstacle map coordinates, return discrete obstacle map coords."""
    x = discretize_val(map_position[0])
    y = discretize_val(map_position[1])
    yaw = discretize_val(map_position[2])
    return x, y, yaw


def discretize_val(val):
    lower_cell_bound = int(val) - 0.5
    upper_cell_bound = int(val) + 0.5
    if lower_cell_bound < val <= upper_cell_bound:
        clamped_val = int(val)
    else:
        if val > 0:
            clamped_val = int(val) + 1
        else:
            clamped_val = int(val) - 1
    return clamped_val


def from_continuous_obstacle_map_pos(map_position, navmap):
    x_pos = map_position[0] * navmap.resolution + navmap.min_xy[0]
    y_pos = map_position[1] * navmap.resolution + navmap.min_xy[1]
    yaw = map_position[2] * navmap.yaw_resolution
    return x_pos, y_pos, yaw


def from_image_path(path_to_image_file, thresh=200):
    map_image = PImage.open(path_to_image_file)
    thresh_func = functools.partial(_thresh_function, thresh=thresh)
    monochrome_image = map_image.convert('L').point(thresh_func, mode='1')
    return ObstacleCheck(np.array(monochrome_image).astype(bool))


def _thresh_function(value, thresh):
    if value > thresh:
        return 255
    else:
        return 0
