/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
/*          Основной поток           */
int main(int argc, char *argv[]) 
{
    if (argc < 4 || argc > 6) 
    {
        fprintf(stderr, "Usage: %s arg1 arg2 arg3 [arg4] [arg5]\n", argv[0]);
        exit(1);
    }
    
    printf("[Parent] Arguments:\n");
    for (int i = 0; i < argc; i++) 
    {
        printf("Arg %d: %s\n", i, argv[i]);
    }
    char domain[256];                                                       // Получение доменного имени
    if (getdomainname(domain, sizeof(domain))) 			  	                // getdomainname - вариант функции
    {
        perror("getdomainname");
        exit(1);
    }
    printf("[Parent] Domain name: %s\n", domain);
    
    if (strlen(domain) == 0) 
    {
        printf("[Parent] Warning: Domain not set. Use 'sudo domainname yyyy'\n");
    }
    char domain_env[300];                                                   // Формирование переменных окружения
    snprintf(domain_env, sizeof(domain_env), "DOMAIN=%s", domain);
    char *envp[] = { domain_env, "TEST=123", NULL };
    char *child_args[] = { "child", argv[1], argv[2], argv[3], NULL };      // Аргументы для дочернего процесса (первые 3 аргумента)
    pid_t pid = fork();
    if (pid == -1) 
    {
        perror("fork");
        exit(1);
    } 
    else if (pid == 0) 
    {                                                                       // Дочерний процесс: вызов execle() с корректным именем файла
        execle("./OS_LIM_LAB4_CHILD", child_args[0], child_args[1], child_args[2], child_args[3], NULL, envp);
        perror("execle");
        exit(1);
    } 
    else 
    {                                                                       // Родительский процесс
        printf("[Parent] PID: %d, PPID: %d\n", getpid(), getppid());
        printf("[Parent] Child PID: %d\n", pid);
        int status;
        pid_t w;
        do {                                                                // Цикл ожидания завершения дочернего процесса, обновляет результат waitpid() каждый раз
            w = waitpid(pid, &status, WNOHANG);
            if (w == 0) 
            {
                printf("[Parent] Waiting...\n");
                usleep(500000);
            } 
            else if (w == -1) 
            {
                perror("waitpid");
                exit(1);
            }
        } while (w == 0);
        if (WIFEXITED(status)) 
        {
            printf("[Parent] Child exited with code: %d\n", WEXITSTATUS(status));
        } 
        else 
        {
            printf("[Parent] Child terminated abnormally\n");
        }
    }
    return 0;
}
/*          Конец            */