
import matplotlib.pyplot as plt
import numpy as np
from loadData import loadData, plotData
from classifyWalk import classifyWalk

fn1 = "walkData1.csv"
fn2 = "walkData2.csv"

classification = list()

for i_fn,fn in enumerate([fn1, fn2]):
	L = loadData(fn)
	C = classifyWalk()
	for i,t in enumerate(L.time):
		readings = [L.accel_x[i], L.accel_y[i], L.accel_z[i], L.gyro_x[i], L.gyro_y[i], L.gyro_z[i]]
		phase = C.classify(readings)
		# print("Phase: %d" %phase)
		classification.append(phase)

	P = plotData(L)
	P.plotClassification(classification)
	plt.title("Classification of Gait Phases for walkData%s" %i_fn)
	plt.savefig('walkData%s_classification.png' %i_fn)
	plt.show()