import matplotlib.pyplot as plt
import numpy as np


ticks = np.genfromtxt('../build/motor_ticks.csv', delimiter=',')
plt.plot(ticks[:, 0], 'bx', label='Left')
plt.plot(ticks[:, 1], 'rx', label='Right')
plt.legend()
plt.show()