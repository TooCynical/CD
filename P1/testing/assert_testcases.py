import commands

correct_output = ['2598', '3048', '2772', '3389', '4035', '4060']

for i in range(5, 11):
    output = commands.getstatusoutput('./../bin/main ./test_' + str(i))[1]
    if not (output == correct_output[i - 5]):
        print "Test failed for test" + str(i)
    else:
        print "Test succeeded for test" + str(i)
