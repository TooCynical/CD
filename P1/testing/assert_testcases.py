# Assert whether the binary ./bin/main produces the correct result for a series
# of instances of the 3D rectilinear steiner tree problem for which the correct
# answer is known.

import commands

bpath = './bin/main'
tfpath = './testing/cases/assertion/'

# First set of tests.
correct_output = ['2598', '3048', '2772', '3389', '4035', '4060']
for i in range(5, 11):
    output = commands.getstatusoutput(bpath + ' ' + tfpath + '/test_' + str(i))[1]
    if not (output == correct_output[i - 5]):
        print "Test failed for test" + str(i)
        print output, correct_output[i - 5]
    else:
        print "Test succeeded for test" + str(i)

# Second set of tests.
correct_output = ['2128', '3008', '3033', '3474', '3090', '3773']
for i in range(5, 11):
    output = commands.getstatusoutput(bpath + ' ' + tfpath + '/testA_' + str(i))[1]
    if not (output == correct_output[i - 5]):
        print "Test failed for testA" + str(i)
        print output, correct_output[i - 5]
    else:
        print "Test succeeded for testA" + str(i)

# Third set of tests.
correct_output = ['1960', '2003', '2541', '4212', '4154', '3710']
for i in range(5, 11):
    output = commands.getstatusoutput(bpath + ' ' + tfpath + '/testB_' + str(i))[1]
    if not (output == correct_output[i - 5]):
        print "Test failed for testB" + str(i)
        print output, correct_output[i - 5]
    else:
        print "Test succeeded for testB" + str(i)

# Fourth set of tests.
correct_output = ['2605', '2988', '3177', '2974', '3259', '4136']
for i in range(5, 11):
    output = commands.getstatusoutput(bpath + ' ' + tfpath + '/testC_' + str(i))[1]
    if not (output == correct_output[i - 5]):
        print "Test failed for testC" + str(i)
        print output, correct_output[i - 5]
    else:
        print "Test succeeded for testC" + str(i)


# Given instances.
correct_output = ['971', '1932', '1645', '2235', '3504']
for i in range(1, 6):
    output = commands.getstatusoutput(bpath + ' ' + tfpath + 'given/i0' + str(i))[1]
    if not (output == correct_output[i - 1]):
        print "Test failed for i0" + str(i)
        print output, correct_output[i - 1]
    else:
        print "Test succeeded for i0" + str(i)
