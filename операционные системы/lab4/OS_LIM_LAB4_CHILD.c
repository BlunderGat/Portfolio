/*          Библиотеки           */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
extern char **environ;
/*          Основной поток           */
int main(int argc, char *argv[]) 
{
    printf("[Child] PID: %d, PPID: %d\n", getpid(), getppid());
    printf("Arguments (%d):\n", argc);              // Вывод аргументов
    for (int i = 0; i < argc; i++) 
    {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    printf("Environment variables:\n");             // Вывод переменных окружения
    for (char **env = environ; *env != NULL; env++) 
    {
        printf("%s\n", *env);
    }
    for (int i = 0; i < argc; i++)                  // Задержка и вывод аргументов
    {
        sleep(1);
        printf("Delayed arg %d: %s\n", i, argv[i]);
    }
    exit(5);                                        // Завершение с кодом 5
}
/*          Конец            */