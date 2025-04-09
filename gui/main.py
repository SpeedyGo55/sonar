import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
plt.rcParams['toolbar'] = 'None'
# SERIAL CONFIGURATION
SERIAL_PORT = "/dev/ttyACM0"  # Change as needed, e.g., "COM3" on Windows
BAUD_RATE = 115200            # Ensure this matches your Pico's configuration

# GRAPH CONFIGURATION
MAX_RANGE = 50  # Maximum distance (cm) for your plot

# Initialize an array for angles 0–180, each starting at MAX_RANGE (indicating no object detected)
distances = np.full(181, MAX_RANGE, dtype=float)

# Initialize serial connection
ser = serial.Serial(SERIAL_PORT, BAUD_RATE, timeout=1)

def update_plot(frame):
    global distances

    # Read available lines from the serial port and update the distance for the corresponding angle.
    while ser.in_waiting:
        try:
            line = ser.readline().decode('utf-8').strip()
            if not line:
                continue
            # Expecting "angle ; distance"
            parts = line.split(';')
            if len(parts) < 2:
                continue
            angle = int(parts[0].strip())
            measured_distance = int(parts[1].strip())
            print(measured_distance)
            if measured_distance==-1:
                measured_distance = MAX_RANGE
            if 0 <= angle <= 180:
                distances[angle] = measured_distance
        except Exception:
            continue

    # Prepare the data for the polar plot.
    angles_deg = np.arange(0, 181, 1)
    theta = np.radians(angles_deg)
    width = np.radians(1)  # each bar covers 1 degree

    ax.clear()

    # Limit plot to 0° to 180°
    ax.set_thetamin(0)
    ax.set_thetamax(180)

    # Set radial axis so that 0 is at the center and MAX_RANGE is at the outer edge.
    ax.set_ylim(0, MAX_RANGE)

    # Define the stacked bar components:
    # Green bar: represents free space (from 0 to the measured distance)
    green_heights = distances
    # Red bar: fills from the measured distance to MAX_RANGE
    red_heights = MAX_RANGE - distances

    # Plot the green (inner) bars.
    ax.bar(theta, green_heights, width=width, bottom=0, color='green', edgecolor='none')
    # Plot the red bars on top.
    ax.bar(theta, red_heights, width=width, bottom=green_heights, color='red', edgecolor='none')

# Set up the polar subplot.
fig = plt.figure(num=" ")
ax = fig.add_subplot(111, polar=True)

# Use matplotlib animation to update the chart in real time.
ani = animation.FuncAnimation(fig, update_plot, interval=100)
plt.show()
