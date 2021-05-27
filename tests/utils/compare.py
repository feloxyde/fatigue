#!/bin/python3

# Copyright 2021 Felix Bertoni
#
# SPDX-License-Identifier: MIT

from sys import argv
from subprocess import Popen, PIPE

if len(argv) != 3 :
    exit(1)

print(argv)

cmd = argv[1] 
p = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
stdout, stderr = p.communicate()

#with open("expected.txt", "w+") as f:
#    f.write(stdout.decode())

with open(argv[2], 'r') as f:
    expected = f.read().splitlines() # Read whole file in the file_content string
    
result = stdout.decode().splitlines()

diff = False
line = 1
details = ""
rfinished = False
efinished = False
while rfinished and efinished :
    resLine = ""
    expLine = ""
    if len(result) < line -1 :
        resLine = result[line-1]
    else :
        rfinished = True
    if len(expected) < line -1 :
        expLine = result[line-1]
    else : 
        efinished = True

    if resLine != expLine or rfinished != efinished :
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
    print(result)
    print("##############")
    print("\n")
    print("### EXPECTED ###")
    print(expected)
    print("################")
    print("\n")
    print(details)
    exit(1)
else :
    exit(0)

