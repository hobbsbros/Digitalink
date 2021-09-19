# Plotting results from the pen accelerometer

import serial
import matplotlib.pyplot as plt

arduino = serial.Serial("COM5")

while True:
    line = str(arduino.readline()).decode("utf-8")
    print(line)
