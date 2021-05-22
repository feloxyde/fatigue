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
print(stdout.decode())

with open(argv[2], 'r') as f:
    file_content = f.read() # Read whole file in the file_content string

print(file_content)

if(file_content == stdout.decode()):
    print("PASSED")
    exit(0)
else :
    print("FAILED")
    exit(1)

