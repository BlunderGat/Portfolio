/*          Библиотеки           */
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
/*          Флаги процессов          */
int FlagThread1 = 0;
int FlagThread2 = 0;
pthread_t Thread1, Thread2;
/*          Функция процесса 1           */
void* ThreadProc1(void* arg) 
{
    char Symbol = *(char*)arg;                                              // Получаем символ
    int Count = 0;
    while (FlagThread1 == 0) 
    {
        putchar(Symbol);
        fflush(stdout); 
        sleep(1);
        Count++;
    }
    return (void*)(intptr_t)Count;                                          // Возвращаем количество итераций
}
/*          Функция процесса 2           */
void* ThreadProc2(void* arg) 
{
    char Symbol = *(char*)arg;                                              // Получаем символ
    int Count = 0;
    while (FlagThread2 == 0) 
    {
        putchar(Symbol);
        fflush(stdout); 
        sleep(1);
        Count++;
    }
    return (void*)(intptr_t)Count;                                          // Возвращаем количество итераций
}
/*          Основной поток           */
int main()
{
    char Arg1 = 'A';                                                        // Параметры для потоков
    char Arg2 = 'B';
    pthread_attr_t Attr;                                                    // Работа с detach state
    pthread_attr_init(&Attr);                                               // Инициализация атрибутов
    
    int DetachState;                                                        // Получаем и выводим значение detach state по умолчанию
    pthread_attr_getdetachstate(&Attr, &DetachState);
    printf("Default detach state: %s\n", 
          (DetachState == PTHREAD_CREATE_JOINABLE) ? "JOINABLE" : "DETACHED");
    
    pthread_attr_setdetachstate(&Attr, PTHREAD_CREATE_DETACHED);            // Меняем на DETACHED для Thread1
    
                                                                            // Создаём потоки с параметрами
    pthread_create(&Thread1, &Attr, ThreadProc1, &Arg1);                    // С атрибутом DETACHED
    pthread_create(&Thread2, NULL, ThreadProc2, &Arg2);                     // С атрибутом по умолчанию

    pthread_attr_destroy(&Attr);                                            // Уничтожаем атрибут
    getchar();
    FlagThread1 = 1;
    FlagThread2 = 1;
    void *result1, *result2;                                                // Получаем результаты работы потоков
    int JoinRes;
    JoinRes = pthread_join(Thread1, &result1);                              // Пытаемся присоединить DETACHED-поток
    if (JoinRes != 0)
        printf("Thread1 join error: %s\n", strerror(JoinRes));
    else
        printf("Thread1 iterations: %ld\n", (intptr_t)result1);
    JoinRes = pthread_join(Thread2, &result2);                              // Присоединяем обычный поток
    if (JoinRes != 0)
        printf("Thread2 join error: %s\n", strerror(JoinRes));
    else
        printf("Thread2 iterations: %ld\n", (intptr_t)result2);
    printf("\nThreads Completed\n");
    getchar();
    return 0;
}
/*          Конец            */
/*  
Суть атрибута detach state:
Joinable: Требует вызова pthread_join для освобождения ресурсов.
Detached: Ресурсы освобождаются автоматически. Вызов pthread_join невозможен.
*/