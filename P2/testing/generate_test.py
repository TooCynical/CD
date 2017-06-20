# Generate a random instance of the 3D rectilinear steiner tree problem.

import random
import sys

for j in range(1, 25):
    f = open('random_test_' + str(j), 'w')
    sys.stdout = f
    print 5
    for i in range(5):
        print random.randint(1, 10), random.randint(1, 10)
    f.close()