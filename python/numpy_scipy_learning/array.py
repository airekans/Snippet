import numpy as np

a = np.array([1, 2, 3, 4])
print a

b = np.array([[1, 2], [3, 4]])
print b

a = a.reshape((2, 2))
print a
print a.dot(b)
