import numpy as np
import pdb
import matplotlib.pyplot as plt


HISTORICAL_KEEP = 100
MIN_LIM = 10




class classifyWalk:
	def __init__(self):
		self.i = 0
		self.readings = list()
		self.current_classification = 3 # assume it starts in midstance


	def classify(self, r_in):
		# r_in should be a list or array with data in the following order
		# # [accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z]
		# classifications are the following:
		# # -1: unknown
			#1: Heel Strike
		 	#2: Foot Flat
		  	#3: Mid stance
			#4: Heel Off
			#5: Toe Off
			#6: Mid Swing

		self.readings.append(r_in)
		if len(self.readings) > HISTORICAL_KEEP:
			self.readings.pop(0)

		if len(self.readings) > MIN_LIM:
			important_vals = np.array(self.readings[-10:])
			avg_gyro = np.mean(important_vals[:,3:], axis = 0)
			avg_all = np.mean(important_vals, axis=0)
			delta_x_rot = self.readings[-1][3] - self.readings[-2][3]
			delta_y_acc = self.readings[-1][1] - self.readings[-2][1]
			if self.readings[-1][1] > 0 and self.readings[-2][1] < 0:
				y_acc_change = 1
			else:
				y_acc_change = 0
			accel = np.sign(self.readings[-1][1]) * (self.readings[-1][0]**2 + self.readings[-1][1]**2 + self.readings[-1][2]**2)**(0.5) # acceleration in 3D space, forward vs. backward determined by y acceleration

			if ((self.current_classification == 6  or self.current_classification == 1) and y_acc_change):
				# foot was slowing down to make contact, but then as it rotates about heel, toes experience forward acceleration
				self.current_classification = 1

			elif ((self.current_classification == 1  or self.current_classification == 2) and self.readings[-1][1] < 0):
				# foot rolls toward the center of the body during foot flat phase
				self.current_classification = 2

			elif ((self.current_classification == 2  or self.current_classification == 3) and (abs(important_vals) < 5).all()):
				# minimal amount of movement -- avoid noisiness of accelerometer
				self.current_classification = 3 # mid stance

			elif ((self.current_classification == 3  or self.current_classification == 4) and self.readings[-1][3] < -2):
				# know that heel off comes after mid stance
				# large negative rotation of foot indicates heel off
				self.current_classification = 4 # heel off

			elif ((self.current_classification == 4 or self.current_classification == 5) and self.readings[-1][2] > 0 and self.readings[-1][3] > -5):
				# know that toe off comes after heel off
				# increase in z height and no more rotation indicates toe off
				self.current_classification = 5

			elif ((self.current_classification == 5 or self.current_classification == 6) and abs(accel) > 10):
				# know that mid-swing comes after toe off
				# during mid-swing the foot should be moving the fastest, mostly in y and z
				self.current_classification = 6
			# else:
			# 	print("No Phase Change")

		return self.current_classification



if __name__ == '__main__':
	C = classifyWalk()
