#!/bin/bash
gcc -c OS_LIM_LAB2_2.c
gcc -o OS_LIM_LAB2_2 OS_LIM_LAB2_2.o -lpthread -lrt
./OS_LIM_LAB2_2