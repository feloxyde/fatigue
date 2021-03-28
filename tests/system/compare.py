#!/bin/python3

from sys import argv
from subprocess import Popen, PIPE

if len(argv) != 3 :
    exit(1)

cmd = argv[1] 
p = Popen(cmd, stdout=PIPE, stderr=PIPE, shell=True)
stdout, stderr = p.communicate()

with open("expected.txt", "w+") as f:
    f.write(stdout.decode())
print(stdout.decode())

with open(argv[2], 'r') as f:
    file_content = f.read() # Read whole file in the file_content string

if(file_content == stdout.decode()):
    print("PASSED")
else :
    print("FAILED")