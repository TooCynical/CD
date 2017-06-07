# Generate a random instance of the 3D rectilinear steiner tree problem.

import random
import sys

for j in range(1, 25):
    f = open('bigtest_' + str(j), 'w')
    sys.stdout = f
    print 20
    for i in range(20):
        print random.randint(0, 999), random.randint(0, 999), random.randint(0, 999)
    f.close()