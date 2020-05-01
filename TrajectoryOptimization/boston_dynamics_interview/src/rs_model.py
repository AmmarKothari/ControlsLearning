import functools
import math

from src import anum


class RSModel:
    def step(self, start, steering_angle, drive_distance):
        x_new = start[0] + drive_distance * math.cos(start[2])
        y_new = start[1] + drive_distance * math.sin(start[2])
        yaw_new = [start[2] + anum.sign(steering_angle) * math.tan(abs(steering_angle)) % (2 * math.pi)
        return [x_new, y_new, yaw_new]

    def get_path_from_control(
            self, start, control_path):
        path = [start]
        current_pos = start
        for (turn_amount, drive_distance) in control_path:
            current_pos = self.step(current_pos, turn_amount, drive_distance)
            path.append(current_pos)
        return path


class BicycleModelConfig:
    def __init__(self, max_steering_angle, max_velocity):
        self.max_steering_angle = max_steering_angle
        self.max_velocity = max_velocity
        self.length_rear = 1.0
        self.length_front = 1.0



class BicycleModel:
    def __init__(self, config):
        # type: (BicycleModelConfig) -> None
        self.config = config
        self.steering_angles = [-self.config.max_steering_angle, 0.0, self.config.max_steering_angle]
        self.velocities = [-self.config.max_velocity, self.config.max_velocity]
        self.calc_all_betas()
        self.calc_all_turning_rates()

    def calc_all_betas(self):
        # Beta = current velocity of the center of mass
        self.betas = []
        for steering_angle in self.steering_angles:
            self.betas.append(math.atan(self.config.length_rear / (self.config.length_front + self.config.length_rear) * math.tan(steering_angle)))

    def calc_all_turning_rates(self):
        self.yaw_rates = []
        for beta in self.betas:
            for velocity in self.velocities:
                self.yaw_rates.append(velocity / self.config.length_rear * math.sin(beta))
