#!/bin/bash

gcc OS_LIM_LAB7V1_1.c -o OS_LIM_LAB7V1_1 -lrt -lpthread
gcc OS_LIM_LAB7V1_2.c -o OS_LIM_LAB7V1_2 -lrt -lpthread
# OS_LIM_LAB7V1_2 - это файл получатель, его надо запустить перед оиправителем, иначе
# OS_LIM_LAB7V1_1 - отправитель - будет отправлять в никуда
echo "В первом терминале сначала запустите:  ./OS_LIM_LAB7V1_2"
echo "Затем во втором терминале запустите:   ./OS_LIM_LAB7V1_1"
