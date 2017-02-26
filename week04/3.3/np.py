import numpy as np
import time

with open('../mat10000.dat') as f:
   	polyShape = []
	for line in f:
		line = line.split() # to deal with blank 
		if line:            # lines (ie skip them)
			line = [int(i) for i in line]
			polyShape.append(line)
	del polyShape[0]

	start = time.time()
	mx = np.matrix(polyShape)
	mx*mx
	end = time.time()
	print(end - start)