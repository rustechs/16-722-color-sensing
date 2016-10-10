#!/usr/bin/env python3

# mms-classifier.py
# HW 3: Colorimeter Project
# 16-722
# 
# Alexander Volkov

import numpy as numpy
import serial
from sklearn import neighbors
import pdb

# System Params
k_neighbors = 5 # How many neighbors to consider?

def main():
	# Setup

	# Set up serial interface
	ser = serial.Serial(
		port = '/dev/ttyACM0',
		baudrate = 9600,
		timeout = 1.0
	)

	ser.isOpen()
	ser.reset_input_buffer()
	ser.reset_output_buffer()

	# Instantiate the kNN object
	kNN = neighbors.KNeighborsClassifier(k_neighbors, weights='uniform')

	# Load manually labeled training data
	trainData = open('dataset.txt','r')

	# Parse training data
	for trainSample in trainData:
		
		splitSamp = trainSample.partition(':')

		sampSig = list(map(int,((splitSamp[0]).strip('[]')).split(',')))
		sampLabel = splitSamp[2]
		
		trainedSigs.append()
		trainedLabels.append()

	# Close training data file
	trainData.close()

	# Train kNN on training data
	kNN.fit(trainedSigs,trainedLabels)

	# Infinite Loop
	while(True):
		cmd = input("\n\nType 'q' to quit or hit any other key to start a measurement: ")
		if (cmd == 'q'):
			# Quit program
			print("Quitting...\n\n")
			ser.close()
			exit() 
		else:
			# Begin a measurement
			print("Taking a measurement...")
			ser.write(b'm')
			ser.flush()

			# Read response as byte array
			# Repeat until valid measurement arrives
			while(True):
				resp = (ser.readline())[:-2]
				if (resp != b''):
					break

			# Convert byte array to ascii string
			respStrList = (resp.decode('ascii')).split(',')

			# Parse string into list of integers
			meas = list(map(int,(respStrList)))

			print("Measured Signature: ", respStrList)

			# Predict a class label for the measurement
			print("Predicted Class: ", kNN.predict([meas]))

if __name__ == '__main__':
	main()