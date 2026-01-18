#!/bin/bash
gcc -c OS_LIM_LAB4_CHILD.c
gcc -c OS_LIM_LAB4_PARENT.c
gcc -o OS_LIM_LAB4_CHILD OS_LIM_LAB4_CHILD.o -lpthread -lrt
gcc -o OS_LIM_LAB4_PARENT OS_LIM_LAB4_PARENT.o -lpthread -lrt
./OS_LIM_LAB4_PARENT 1 2 3 4 
