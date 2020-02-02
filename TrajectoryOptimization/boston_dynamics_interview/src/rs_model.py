import functools
import math

class RSModel:
    def step(self, start, steering_angle, drive_distance):
        # theta = self._calc_theta(turning_radius, drive_distance)
        x_new = start[0] + drive_distance * math.cos(start[2])
        y_new = start[1] + drive_distance * math.sin(start[2])
        yaw_new = start[2] + steering_angle
        return x_new, y_new, yaw_new

    @functools.lru_cache()
    def _calc_theta(self, turning_radius, drive_distance):
        return drive_distance / turning_radius * 2 * math.pi

    def get_path_from_control(self, start, control_path):
        path = [start]
        current_pos = start
        for (turn_amount, drive_distance) in control_path:
            current_pos = self.step(current_pos, turn_amount, drive_distance)
            pdb.set_trace()
            path.append(current_pos)
        return path