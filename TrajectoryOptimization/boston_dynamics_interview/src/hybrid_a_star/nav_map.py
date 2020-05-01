import math

NUM_DISCRETE_YAWS = 40
DEFAULT_YAW_RESOLUTION = 2 * math.pi / NUM_DISCRETE_YAWS


class NavMap:
    def __init__(self, grid, resolution=0.1, min_xy=(0.0, 0.0), yaw_resolution=DEFAULT_YAW_RESOLUTION):
        self.min_xy = min_xy
        self._grid = grid
        self.resolution = resolution
        self.yaw_resolution = yaw_resolution

    @property
    def min_idxs(self):
        return [0, 0]

    @property
    def max_idxs(self):
        return [self._grid.shape[0] - 1, self._grid.shape[1] - 1]

    def __getitem__(self, item):
        return self._grid[item]
