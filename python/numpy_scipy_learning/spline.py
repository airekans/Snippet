import numpy as np
import scipy as sp
from scipy.interpolate import UnivariateSpline
import matplotlib.pyplot as plt

x = [1, 2, 3, 4, 5, 6, 7]
y = [1, 4, 9, 16, 26, 30, 51]

# Test Spline
trajectory = np.array([x, y], dtype=float)
print trajectory
plt.plot(trajectory[0], trajectory[1])
plt.show()

s = sp.interpolate.UnivariateSpline(trajectory[0], trajectory[1])

smoothX = trajectory[0]
smoothTrajectory = np.array([smoothX, s(smoothX)])
print smoothTrajectory

# Results
plt.subplot(1, 2, 1)
plt.plot(trajectory[0])
plt.plot(smoothTrajectory[0])
plt.subplot(1, 2, 2)
plt.plot(trajectory[1])
plt.plot(smoothTrajectory[1])
plt.show()


# Test Spline 2
s = sp.interpolate.UnivariateSpline(trajectory[0], trajectory[1], s=1)

smoothX = trajectory[0]
smoothTrajectory = np.array([smoothX, s(smoothX)])

# Results
plt.subplot(1, 2, 1)
plt.plot(trajectory[0])
plt.plot(smoothTrajectory[0])
plt.subplot(1, 2, 2)
plt.plot(trajectory[1])
plt.plot(smoothTrajectory[1])
plt.show()

# Test Spline 3
s = sp.interpolate.UnivariateSpline(trajectory[0], trajectory[1], s=2)

smoothX = trajectory[0]
smoothTrajectory = np.array([smoothX, s(smoothX)])

# Results
plt.subplot(1, 2, 1)
plt.plot(trajectory[0])
plt.plot(smoothTrajectory[0])
plt.subplot(1, 2, 2)
plt.plot(trajectory[1])
plt.plot(smoothTrajectory[1])
plt.show()
