# Test whether the two given binaries produce the same output
# when pointed to a series of randomly generated instances of
# the 3D rectilinear steiner tree problem.

import random
import sys
import commands

prog1 = raw_input("Enter 1st binary path: ")
prog2 = raw_input("Enter 2nd binary path: ")

n = input("Enter number of test cases: ")
m = input("Enter max terminals: ")
l = input("Enter min terminals: ")

testfile = "testfile" + str(random.randint(0,100000))
ret = sys.stdout

for i in range(n):
    for j in range(l, m+1):
        f = open(testfile, 'w')
        sys.stdout = f
        print j
        for k in range(j):
            print random.randint(0, 999), random.randint(0, 999), random.randint(0, 999)
        f.close()
        sys.stdout = ret
        result1 = commands.getstatusoutput(prog1 + " " + testfile)[1]
        result2 = commands.getstatusoutput(prog2 + " " + testfile)[1]

        if result1 == result2:
            print str(testfile) + ": Test succeeded.", result1, result2, "(", j, "terminals )"
        else:
            print str(testfile) + ": Test failed.", result1, result2, "(", j, "terminals )"
            exit(0)
