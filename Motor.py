class Motor(object): #class to simulate an electric motor
	def __init__(self, torque_to_voltage):
		self.conv = torque_to_voltage
		self.v_min = 2
		self.v_max = 12

	def pwm_convert(self, pwm): # converts pwm to torque
		pwm = int(pwm)
		if pwm > 255.0:
			pwm = 255
		elif pwm < -255:
			pwm = -255
		# convert pwm to voltage
		v = pwm / 255.0 * (self.v_min - self.v_max)
		T = (v - self.v_min)/self.conv
		# print(T)
		return T