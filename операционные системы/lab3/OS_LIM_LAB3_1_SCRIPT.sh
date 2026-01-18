#!/bin/bash
gcc -c OS_LIM_LAB3.c
gcc -o OS_LIM_LAB3 OS_LIM_LAB3.o -lpthread -lrt
./OS_LIM_LAB3 1