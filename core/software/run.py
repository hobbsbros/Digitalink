# Plotting results from the pen accelerometer

import serial
import matplotlib.pyplot as plt
import time

arduino = serial.Serial("COM5")

x = []
y = []

for i in range(1000):
    line = str(arduino.readline()).decode("utf-8")
    arr = line.split(",")
    x.append(arr[0])
    y.append(arr[1])
    time.sleep(0.01)

plt.plot(x, y)
plt.show()
    
