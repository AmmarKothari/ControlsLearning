import math


class FourDirection:
    def step(self, position, distance=1, turning_angle=0):
        # Generator
        x_pos = (position[0] + distance, position[1], position[2])
        yield x_pos
        x_neg = (position[0] - distance, position[1], position[2])
        yield x_neg
        y_pos = (position[0], position[1] + distance, position[2])
        yield y_pos
        y_neg = (position[0], position[1] - distance, position[2])
        yield y_neg


class EightDirection:
    def step(self, position, distance=1, turning_angle=0):
        # Generator
        for x_dist_normalized in [-1, 0.0, 1]:
            for y_dist_normalized in [-1, 0.0, 1]:
                total_dist_norm = math.sqrt(x_dist_normalized ** 2 + y_dist_normalized ** 2)
                if total_dist_norm == 0.0:
                    x_dist = y_dist = 0.0
                else:
                    x_dist = x_dist_normalized / total_dist_norm * distance
                    y_dist = y_dist_normalized / total_dist_norm * distance
                yield (position[0] + x_dist, position[1] + y_dist, position[2])
