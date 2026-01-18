#!/bin/bash
gcc -c OS_LIM_LAB2_3.c
gcc -o OS_LIM_LAB2_3 OS_LIM_LAB2_3.o -lpthread -lrt
./OS_LIM_LAB2_3