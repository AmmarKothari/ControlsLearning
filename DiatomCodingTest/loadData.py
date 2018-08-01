
import csv
import pdb
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.patches as ptc
from matplotlib.collections import PatchCollection

G = 9.8 # m/s^2 -- gravity acceleration
ACC_B2ACC = G/16384.0   # convert from bits to degrees/second
GYR_B2GYR = 1.0/250.0   # convert from bits to m/s^2

class loadData:
	def __init__(self, fn):
		self.fn = fn
		self.time = np.zeros(1)
		self.readCSV(fn)
		self.calculateAcceleration()
		# self.integrateReadings()

	def readCSV(self, fn):
		with open(fn, 'rb') as csvfile:
			reader = csv.DictReader(csvfile)
			row = next(reader)
			# initialize all the variables
			self.time = list() # ms
			# +/- 0.2 * G for 16384 bits/G
			self.accel_x = list()
			self.accel_y = list()
			self.accel_z = list()
			# +/- 250 degrees/second for 131 for each degree/sec
			self.gyro_x = list()
			self.gyro_y = list()
			self.gyro_z = list()

			for row in reader:
				self.time.append(int(row['Timestamp (ms)']))
				self.accel_x.append(int(row["accel.x"]))
				self.accel_y.append(int(row["accel.y"]))
				self.accel_z.append(int(row["accel.z"]))

				self.gyro_x.append(int(row["gyro.x"]))
				self.gyro_y.append(int(row["gyro.y"]))
				self.gyro_z.append(int(row["gyro.z"]))

			self.time = np.array(self.time)/1000.0 # seconds
			
			self.accel_x = np.array(self.accel_x) * ACC_B2ACC # m/s^2
			self.accel_y = np.array(self.accel_y) * ACC_B2ACC
			self.accel_z = np.array(self.accel_z) * ACC_B2ACC


			self.gyro_x = np.array(self.gyro_x) * GYR_B2GYR # deg/s
			self.gyro_y = np.array(self.gyro_y) * GYR_B2GYR
			self.gyro_z = np.array(self.gyro_z) * GYR_B2GYR

	def calculateAcceleration(self):
		self.accel = [(x**2 + y**2 + z**2)**(0.5) for x,y,z in zip(self.gyro_x, self.gyro_y, self.gyro_z)]
		# pdb.set_trace()

	def integrateReadings(self):
		# pdb.set_trace()
		self.dt = self.time[:-1] - self.time[1:]

class plotData:
	def __init__(self, d):
		# d is loadData object
		self.D = d

	def plotAccelerations(self):
		plt.figure()
		plt.plot(self.D.time, self.D.accel_x, 'rx', label='X')
		plt.plot(self.D.time, self.D.accel_y, 'bx', label='Y')
		plt.plot(self.D.time, self.D.accel_z, 'kx', label='Z')
		plt.legend()

	def plotGyros(self):
		# plt.figure()
		plt.plot(self.D.time, self.D.gyro_x, 'ro', label='X')
		plt.plot(self.D.time, self.D.gyro_y, 'bo', label='Y')
		plt.plot(self.D.time, self.D.gyro_z, 'ko', label='Z')
		plt.legend()

	def plotClassification(self, classification):
		color_dict = {-1:'r', 
					   1:'b',
					   2:'g',
					   3:'k',
					   4:'y',
					   5:'c',
					   6:'m'}

		name_dict = {-1: "unknown",
					  1: "Heel Strike",
					  2: "Foot Flat",
					  3: "Mid stance",
					  4: "Heel Off",
					  5: "Toe Off",
					  6: "Mid Swing"}


		color_list = list()
		for i,t in enumerate(self.D.time):
			color_list.append(color_dict[classification[i]])

		fig, ax = plt.subplots(1)
		fig.set_size_inches(15,10)


		ax.plot(self.D.time, self.D.accel_x, 'x')
		ax.plot(self.D.time, self.D.accel_y, 'x')
		ax.plot(self.D.time, self.D.accel_z, 'x')

		ax.plot(self.D.time, self.D.gyro_x, 'o')
		ax.plot(self.D.time, self.D.gyro_y, 'o')
		ax.plot(self.D.time, self.D.gyro_z, 'o')

		# plt.legend('x', 'y', 'z')

		patch_list = []
		prev_class = 3
		left_index = 0
		right_index = 0
		for i,t in enumerate(self.D.time):
			right_index = t
			if prev_class != classification[i]:
				# record old class limits
				patch_list.append([left_index, right_index, prev_class])
				left_index = t

			prev_class = classification[i]
		patch_list.append([left_index, right_index, prev_class]) # capture last class
		y_lim = ax.get_ylim()
		legend_tracker = []

		for patch in patch_list:
			if patch[2] in legend_tracker:
				legend_name = "_nolegend_"
			else:
				legend_name = name_dict[patch[2]]
				legend_tracker.append(patch[2])
			rect = ptc.Rectangle((patch[0], 0.5*y_lim[0]),
									patch[1]-patch[0], 0.5*(y_lim[1]-y_lim[0]),
									fill=True,
									color=color_dict[patch[2]],
									alpha=0.4,
									label=legend_name)
			if patch[2] == 1:
				ax.axvline(x=patch[0])
			ax.add_patch(rect)
			plt.xlabel('Time (s)')
			plt.ylabel('Sensor Readings (m/s^2 or deg/sec)')
		plt.legend()

		




if __name__ == '__main__':
	D = loadData("walkData2.csv")
	P = plotData(D)
	P.plotAccelerations()
	P.plotGyros()
	plt.show()