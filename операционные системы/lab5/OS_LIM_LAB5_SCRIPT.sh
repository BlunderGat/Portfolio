#!/bin/bash
rm -f writer1 writer2 shared.txt
sudo rm -f /dev/shm/sem.mysem
gcc OS_LIM_LAB5_1.c -o OS_LIM_LAB5_1 -lrt -pthread
gcc OS_LIM_LAB5_2.c -o OS_LIM_LAB5_2 -lrt -pthread
echo "Run in three different terminals:"
echo "1. ./OS_LIM_LAB5_1"
echo "2. ./OS_LIM_LAB5_2"
echo "3. tail -f shared.txt"