#!/bin/bash

gcc OS_LIM_LAB8_S.c -o OS_LIM_LAB8_S -lrt -lpthread
gcc OS_LIM_LAB8_C.c -o OS_LIM_LAB8_C -lrt -lpthread

echo "В первом терминале сначала запустите:  ./OS_LIM_LAB8_S"
echo "Затем во втором терминале запустите:   ./OS_LIM_LAB8_C"
