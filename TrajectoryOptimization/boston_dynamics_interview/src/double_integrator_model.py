import numpy as np


class DoubleIntegrator:
    num_states = 4  # type: int
    num_control_states = 2  # type: int

    def step(self, start, acceleration, dt):
        #  (Sequence[float], Sequence[float], float) -> np.ndarray
        dx_new = start[2] + acceleration[0] * dt
        dy_new = start[3] + acceleration[1] * dt
        x_new = start[0] + start[2] * dt + 0.5 * acceleration[0] * dt**2
        y_new = start[1] + start[3] * dt + 0.5 * acceleration[1] * dt**2
        return np.array((x_new, y_new, dx_new, dy_new))

    def get_path_from_control(self, start, control_path, dt):
        path = [start]
        current_pos = start
        for acceleration in control_path:
            current_pos = self.step(current_pos, acceleration, dt)
            path.append(current_pos)
        return path