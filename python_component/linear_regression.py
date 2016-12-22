import numpy as np

def linear_regression(x, y):
	regr = np.polyfit(x, y, 1)
	return regr
