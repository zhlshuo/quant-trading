import numpy as np

def linear_regression(x, y):
	regr = np.polyfit(x, y, 1)
	print(x)
	print(y)
	ret = list(regr)
	print(type(ret[0]), ret[0])
	print(type(ret[1]), ret[1])
	return list(regr)

if __name__=='__main__':
	print(linear_regression([1,2,3], [3,4,5]))
