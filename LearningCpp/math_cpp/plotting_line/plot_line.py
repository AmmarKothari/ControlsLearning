"""Read a csv file and plot it"""


import matplotlib.pyplot as plt
import numpy as np
import pdb

INPUT_FILE = 'output_file.csv'

input_data = np.genfromtxt(INPUT_FILE)

plt.plot(input_data, 'rx')

plt.show()

