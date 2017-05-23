import random
import sys


for j in range(5, 11):
    f = open('test_' + str(j), 'w')
    sys.stdout = f
    print j
    for i in range(j):
        print random.randint(0, 999), random.randint(0, 999), random.randint(0, 999)
    f.close()