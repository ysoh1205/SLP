#!/bin/bash
make
echo "$1 $2" | ./w3.out > output.txt
plus=`expr $1 + $2`
minus=`expr $1 - $2`
echo "$plus $minus" > answer.txt
diff output.txt answer.txt>result.txt
make clean
