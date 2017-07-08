# Generate a random instance of the 3D rectilinear steiner tree problem.

import random
import sys

for j in range(1, 25):
    f = open('random_test_' + str(j), 'w')
    sys.stdout = f
    print 6
    for i in range(6):
        print random.randint(1, 100), random.randint(1, 100)
    f.close()
