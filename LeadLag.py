import control
import matplotlib.pyplot as plt
import numpy as np
from GeneralController import GeneralController


class LeadLagController(GeneralController):
	def __init__(self, t, Kc = 1):
		super(self, GeneralController).__init__(t)
		self.Kc = Kc
		self.characteristicEquation()

	def setQTarget(self, q):
		super(self, GeneralController).setQTarget(q)

	def characteristicEquation(self):