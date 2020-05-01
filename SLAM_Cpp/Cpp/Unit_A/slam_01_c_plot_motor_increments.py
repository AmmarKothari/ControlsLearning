import matplotlib.pyplot as plt
import numpy as np


ticks = np.genfromtxt('../build/relative_motor_ticks.csv', delimiter=',')
plt.plot(ticks[:, 0], 'b-', label='Left')
plt.plot(ticks[:, 1], 'r-', label='Right')
plt.legend()
plt.show()