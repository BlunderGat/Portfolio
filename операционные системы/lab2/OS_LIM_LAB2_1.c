/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
/*          Флаги потоков          */
volatile int FlagThread1 = 0;              
volatile int FlagThread2 = 0;              
/*          Функция потока 1           */
void* ThreadProc1(void* arg) 
{
    while (!FlagThread1) 
    {
        for (int i = 0; i < 10; i++) 
        { // Начало критического участка 
            printf("1");
            fflush(stdout);                                     // Очищаем буфер вывода
            sleep(1);
        } // Конец критического участка
        
        sleep(1);                                               // Работа вне критического участка
    }
    return NULL;
}
/*          Функция потока 2           */
void* ThreadProc2(void* arg) 
{
    while (!FlagThread2) 
    {
        for (int i = 0; i < 10; i++) 
        { // Начало критического участка
            printf("2");
            fflush(stdout);                                     // Очищаем буфер вывода
            sleep(1);
        } // Конец критического участка
        
        sleep(1);                                               // Работа вне критического участка
    }
    return NULL;
}
/*          Основной поток           */
int main() 
{
    pthread_t Thread1, Thread2;
    pthread_create(&Thread1, NULL, ThreadProc1, NULL);          // Создание потоков
    pthread_create(&Thread2, NULL, ThreadProc2, NULL);
    getchar();                                                  // Ожидание нажатия клавиши
    FlagThread1 = 1;                                            // Установка флагов завершения
    FlagThread2 = 1;
    pthread_join(Thread1, NULL);                                // Ожидание завершения потоков
    pthread_join(Thread2, NULL);
    printf("\nProgramm Complete\n");
    return 0;
}
/*          Конец            */
/* В данной программе синхронизация отсутствует, поэтому вывод символов будет перемешанным (например, 12121122...) */