


The project contains three files
- main.py: this is used to run the analysis on the gait and generate the final plots
- loadData.py: this file has two classes (loadData and plotData)
	- loadData class reads in the data from the csv files.  NOTE: the csv file were editted so that the first line were the headers
	- plotData class plots raw data as well as the data with the classification boxes
- classifyWalk.py: This file contains a class which classifies the gait based on sensory input.
	- the classify method takes in 6 values from the IMU and outputs an integer which corresponds to a class


Project Depends on the following:
	Python 2.7.12
	Matplotlib 2.0.2
	Numpy 1.13.1