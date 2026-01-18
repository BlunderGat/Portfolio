#!/bin/bash
gcc -c OS_LIM_LAB2_1.c
gcc -o OS_LIM_LAB2_1 OS_LIM_LAB2_1.o -lpthread -lrt
./OS_LIM_LAB2_1