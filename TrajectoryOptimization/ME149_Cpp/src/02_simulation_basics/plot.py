import matplotlib.pyplot as plt
import numpy as np
import os
import textwrap
from mpl_toolkits.mplot3d import Axes3D  # noqa: F401 unused import

PLOT_MARKER_COLOR = "b-"


def get_build_file_path(filename):
    cur_dir = os.path.abspath(os.curdir)
    build_dir = cur_dir.replace('src', 'build')
    return os.path.join(build_dir, filename)


def wrap_str(label_str):
    return '\n'.join(textwrap.wrap(label_str, 20))


def problem_03():
    all_data = np.genfromtxt(get_build_file_path(
        "problem_03.csv"), delimiter=",")

    plt.figure(123)
    ax = plt.subplot(3, 1, 1)
    plt.plot(all_data[:, 0], all_data[:, 1], PLOT_MARKER_COLOR)
    plt.ylabel(wrap_str('Angular Position (rads)'))

    ax = plt.subplot(3, 1, 2)
    plt.plot(all_data[:, 0], all_data[:, 2], PLOT_MARKER_COLOR)
    plt.ylabel(wrap_str('Angular Velocity (rads/s)'))

    ax = plt.subplot(3, 1, 3)
    plt.plot(all_data[:, 0], all_data[:, 3], PLOT_MARKER_COLOR)
    plt.ylabel(wrap_str('Angular Acceleration (rads/s^2)'))

    plt.xlabel('Time (s)')

    plt.savefig(get_build_file_path('problem_03'))


def problem_04():
    all_data = np.genfromtxt(get_build_file_path(
        "problem_04.csv"), delimiter=",")

    fig = plt.figure(123)
    ax = fig.add_subplot(211, projection='3d')
    plt.title("3D Lorenz Plot")
    plt.axis("off")
    ax.scatter(all_data[:, 1], all_data[:, 2], all_data[:, 3])

    plt.subplot(2, 2, 3)
    plt.title('States vs Time')
    plt.plot(all_data[:, 0], all_data[:, 1], 'r-', label='x')
    plt.plot(all_data[:, 0], all_data[:, 2], 'b-', label='y')
    plt.plot(all_data[:, 0], all_data[:, 3], 'g-', label='z')
    plt.xlabel('Time')
    plt.legend()

    plt.subplot(2, 2, 4)
    plt.title('Derivative of states vs Time')
    plt.plot(all_data[:, 0], all_data[:, 4], 'r-', label='dx')
    plt.plot(all_data[:, 0], all_data[:, 5], 'b-', label='dy')
    plt.plot(all_data[:, 0], all_data[:, 6], 'g-', label='dz')
    plt.xlabel('Time')
    plt.legend()

    plt.savefig(get_build_file_path('problem_04'))


problem_03()
problem_04()
# plt.show()
