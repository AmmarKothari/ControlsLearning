import numpy as np
import matplotlib.pyplot as plt

poses = np.genfromtxt("../build/robot_scanner_pose.csv", delimiter=",")
plt.plot(poses[:, 0], poses[:, 1], 'bo')
plt.xlabel('X (mm)')
plt.ylabel('Y (mm)')
plt.title('Position of Robot')
plt.show()

