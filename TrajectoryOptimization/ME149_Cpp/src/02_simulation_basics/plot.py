import matplotlib.pyplot as plt
import numpy as np
import os
import textwrap

PLOT_MARKER_COLOR = "b-"

def get_build_file_path(filename):
    cur_dir = os.path.abspath(os.curdir)
    build_dir = cur_dir.replace('src', 'build')
    return os.path.join(build_dir, filename)

def wrap_str(label_str):
    return '\n'.join(textwrap.wrap(label_str, 20))


all_data = np.genfromtxt(get_build_file_path("problem_03.csv"), delimiter=",")

ax = plt.subplot(3,1,1)
plt.plot(all_data[:, 0], all_data[:, 1], PLOT_MARKER_COLOR)
plt.ylabel(wrap_str('Angular Position (rads)'))

ax = plt.subplot(3,1,2)
plt.plot(all_data[:, 0], all_data[:, 2], PLOT_MARKER_COLOR)
plt.ylabel(wrap_str('Angular Velocity (rads/s)'))

ax = plt.subplot(3,1,3)
plt.plot(all_data[:, 0], all_data[:, 3], PLOT_MARKER_COLOR)
plt.ylabel(wrap_str('Angular Acceleration (rads/s^2)'))

plt.xlabel('Time (s)')
plt.show()

