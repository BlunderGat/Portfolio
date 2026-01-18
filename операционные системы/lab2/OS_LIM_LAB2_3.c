/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <errno.h>
/*          Флаги потоков          */
volatile int FlagThread1 = 0;
volatile int Thread2_flag = 0;
pthread_mutex_t mutex;                                          // Мьютекс для синхронизации
/*          Обработчик сигнала SIGINT          */
void sig_handler(int signo) 
{ 
    printf("\nSIGINT; Complete\n");
    pthread_mutex_unlock(&mutex);                               // Разблокируем мьютекс принудительно
    exit(0);
}
/*          Функция потока 1           */
void* ThreadProc1(void* arg) 
{
    struct timespec ts;
    while (!FlagThread1) 
    { // Цикл неблокирующего захвата мьютекса с таймаутом
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;                                         // Таймаут 1 секунда
        if (pthread_mutex_timedlock(&mutex, &ts) == 0) 
        {
            for (int i = 0; i < 10; i++) 
            { // Начало критического участка
                printf("1");
                fflush(stdout);                                 // Очищаем буфер вывода
                sleep(1);
            } // Конец критического участка
            pthread_mutex_unlock(&mutex);
            sleep(1);                                           // Работа вне критического участка
        } else 
        {                                                       // Таймаут
            if (FlagThread1) break;                             // Если не удалось захватить мьютекс, проверяем флаг
        }
    }
    return NULL;
}
/*          Функция потока 2           */
void* ThreadProc2(void* arg) 
{
    struct timespec ts;
    while (!Thread2_flag) 
    { // Цикл неблокирующего захвата мьютекса с таймаутом
        clock_gettime(CLOCK_REALTIME, &ts);
        ts.tv_sec += 1;                                         // Таймаут 1 секунда
        if (pthread_mutex_timedlock(&mutex, &ts) == 0) 
        {
            for (int i = 0; i < 10; i++) 
            { // Начало критического участка
                printf("2");
                fflush(stdout);                                 // Очищаем буфер вывода
                sleep(1);
            } // Конец критического участка
            pthread_mutex_unlock(&mutex);
            sleep(1);                                           // Работа вне критического участка
        } else 
        {                                                       // Таймаут
            if (Thread2_flag) break;                            // Если не удалось захватить мьютекс, проверяем флаг
        }
    }
    return NULL;
}
/*          Основной поток           */
int main() 
{
    pthread_t Thread1, Thread2;
    pthread_mutex_init(&mutex, NULL);                           // Инициализация мьютекса
    signal(SIGINT, sig_handler);                                // Создание обработчика
    pthread_create(&Thread1, NULL, ThreadProc1, NULL);          // Создание потоков
    pthread_create(&Thread2, NULL, ThreadProc2, NULL);
    getchar();                                                  // Ожидание нажатия клавиши
    FlagThread1 = 1;                                            // Установка флагов завершения
    Thread2_flag = 1;
    pthread_join(Thread1, NULL);                                // Ожидание завершения потоков
    pthread_join(Thread2, NULL);
    pthread_mutex_destroy(&mutex);                              // Уничтожение мьютекса
    printf("\nProgramm Complete\n");
    return 0;
}
/*          Конец            */

/* Если мьютекс не захвачен за это время, поток проверяет флаг завершения и решает, продолжать ли ожидание */