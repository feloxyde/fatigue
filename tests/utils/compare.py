#!/bin/python3

# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

from sys import argv
from subprocess import Popen, PIPE

if len(argv) != 3 :
    exit(1)

cmd = argv[1] 
p = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
stdout, stderr = p.communicate()

#with open("expected.txt", "w+") as f:
#    f.write(stdout.decode())

with open(argv[2], 'r') as f:
    expected = f.read().splitlines() # Read whole file in the file_content string
    
result = stdout.decode().splitlines()

print(result)
print("#####")
print(expected)

diff = False
line = 1
details = ""
rfinished = False
efinished = False
while not rfinished and not efinished :
    resLine = ""
    expLine = ""
    if line -1 < len(result) :
        resLine = result[line-1]
    else :
        rfinished = True
    if  line -1 < len(expected) :
        expLine = expected[line-1]
    else : 
        efinished = True
    if resLine != expLine or rfinished != efinished :
        diff = True
        details += "\nlines " + str(line) + " are different : \n"
        if not rfinished :
            details += "result   : " + resLine + "\n"
        else :
            details += "result string ended\n"
        if not efinished : 
            details += "expected : " + expLine + "\n"
        else :
            details += "expected string ended\n"

    line += 1

if diff : 
    print("the two strings are different :")
    print("### RESULT ###")
    for r in result :
        print(r)   
    print("##############")
    print("\n")
    print("### EXPECTED ###")
    for e in expected :
        print(expected)
    print("################")
    print("\n")
    print(details)
    exit(1)
else :
    exit(0)

