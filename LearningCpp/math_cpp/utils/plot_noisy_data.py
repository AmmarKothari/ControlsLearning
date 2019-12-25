import io
import numpy as np
import matplotlib.pyplot as plt
import click

FN = "one_d_noisy.csv"

def plot_one_d():
	one_d_data = np.genfromtxt("one_d_noisy.csv")
	plt.hist(one_d_data)
	plt.show()

def plot_sin_d():
	sin_data = np.genfromtxt("sin_noisy_data.csv")
	plt.plot(sin_data, 'rx-')
	plt.show()


@click.command()
@click.option('--one_d', 'to_do', flag_value='one_d')
@click.option('--sin_data', 'to_do', flag_value='sin_data')
def plot(to_do):
	print(to_do)
	if to_do == 'one_d':
		plot_one_d()
	if to_do == 'sin_data':
		plot_sin_d()


if __name__ == '__main__':
	plot()

