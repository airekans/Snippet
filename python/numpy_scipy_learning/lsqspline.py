import numpy as np
from numpy import linspace,exp
from numpy.random import randn
from scipy.interpolate import LSQUnivariateSpline
import matplotlib.pyplot as plt

# Test Spline
x = linspace(-3, 3, 100)
y = exp(-x**2) + randn(100) / 10
trajectory = np.array([x, y], dtype=float)
plt.plot(trajectory[0], trajectory[1])
plt.show()

t = [-1, 0, 1]
s = LSQUnivariateSpline(trajectory[0], trajectory[1], t)
#smoothX = linspace(-3, 3, 1000)
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


