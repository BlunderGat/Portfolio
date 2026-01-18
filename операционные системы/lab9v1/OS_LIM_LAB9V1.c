/*          Библиотеки           */
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <unistd.h>
#include <sys/utsname.h>
#include <sys/wait.h>
#define STACK_SIZE (1024 * 1024)
/*          Функция дочернего процесса           */
static int child_func(void *arg) 
{
    if (sethostname("CHILD_HOST", 11) == -1)                    // Изменяем имя хоста в дочернем пространстве
    {
        perror("sethostname");
        exit(EXIT_FAILURE);
    }
    struct utsname uts;                                         // Получаем информацию о системе
    if (uname(&uts) == -1) 
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    printf("Дочерний процесс:\n");
    printf("Hostname: %s\n", uts.nodename);
    printf("PID: %d\n", getpid());
    printf("PPID: %d\n", getppid());
    return 0;
}
/*          Основной поток           */
int main() 
{
    char *stack = malloc(STACK_SIZE);
    if (!stack) 
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    struct utsname parent_uts;                                  // Выводим имя хоста родителя
    if (uname(&parent_uts) == -1) 
    {
        perror("uname");
        exit(EXIT_FAILURE);
    }
    printf("Родительский процесс:\n");
    printf("Hostname: %s\n", parent_uts.nodename);
    printf("PID: %d\n\n", getpid());
    pid_t child_pid = clone(                                    // Создаём дочерний процесс с изолированным UTS
        child_func,
        stack + STACK_SIZE,                                     // Указатель на вершину стека
        CLONE_NEWUTS | SIGCHLD,
        NULL
    );
    if (child_pid == -1) 
    {
        perror("clone");
        exit(EXIT_FAILURE);
    }
    waitpid(child_pid, NULL, 0);                                // Ждём завершения дочернего процесса
    printf("\nРодительский процесс после завершения потомка:\n");
    printf("Hostname: %s\n", parent_uts.nodename);              // Проверяем, что имя хоста родителя не изменилось
    free(stack);
    return 0;
}
/*          Конец            */