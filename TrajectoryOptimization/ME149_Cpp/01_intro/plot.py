import matplotlib.pyplot as plt
import numpy as np


def plot_hw_1(control_points, sample_points):
    circle_radius = 1.0  # same as in the cpp file -- annoying to do seperately
    thetas = np.arange(0, 2*np.pi, 0.1)
    x_circle = np.cos(thetas) * circle_radius
    y_circle = np.sin(thetas) * circle_radius
    plt.plot(control_points[:, 0], control_points[:, 1], 'ro')
    plt.plot(x_circle, y_circle, 'k-')
    plt.plot(sample_points[0, 0], sample_points[0, 1], 'gx')
    plt.plot(sample_points[:, 0], sample_points[:, 1], 'b.')
    plt.axis("equal")

plt.subplot(2, 1, 1)
part_1_control_points = np.genfromtxt("part_1_control_points.csv", delimiter=",")
part_1_sample_points = np.genfromtxt("part_1_sample_fractal_points.csv", delimiter=",")
plot_hw_1(part_1_control_points, part_1_sample_points)



plt.subplot(2, 1, 2)
part_2_control_points = np.genfromtxt("part_2_control_points.csv", delimiter=",")
part_2_sample_points = np.genfromtxt("part_2_sample_fractal_points.csv", delimiter=",")
plot_hw_1(part_2_control_points, part_2_sample_points)

plt.save_fig('hw_1')
plt.show()