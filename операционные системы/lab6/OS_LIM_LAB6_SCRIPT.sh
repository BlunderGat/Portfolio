#!/bin/bash

gcc -o OS_LIM_LAB6_1 OS_LIM_LAB6_1.c -pthread -lrt
gcc -o OS_LIM_LAB6_2 OS_LIM_LAB6_2.c -pthread -lrt

echo "Compilation complete"
echo "Enter into terminal 1: ./OS_LIM_LAB6_1"
echo "Enter into terminal 2: ./OS_LIM_LAB6_2"