/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/select.h>
#include <time.h>
#include <signal.h>
#define SEM_NAME "/mysem"
#define FILE_NAME "shared.txt"                              // Наш файлик
#define CHAR_TO_WRITE '2'                                   // Число
volatile sig_atomic_t flag = 0;
void handle_sigint(int sig) 
{
    flag = 1;
}
/*          Основной поток           */
int main() 
{
    sem_t *sem = NULL;
    int fd;
    struct timespec delay = {1, 0};
    fd_set readfds;
    struct sigaction sa = {0};
    char c = CHAR_TO_WRITE;
    sa.sa_handler = handle_sigint;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGINT, &sa, NULL) == -1) 
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    sem = sem_open(SEM_NAME, O_CREAT, 0644, 1);
    if (sem == SEM_FAILED) 
    {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }
    fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0644);
    if (fd == -1) 
    {
        perror("open");
        sem_close(sem);
        sem_unlink(SEM_NAME);
        exit(EXIT_FAILURE);
    }
    while (!flag) 
    {
        if (sem_wait(sem)) 
        {
            perror("sem_wait");
            break;
        }
        for (int i = 0; i < 10; i++)                        // Критическая секция (исправленный write)
        {
            if (write(fd, &c, 1) == -1)                     // Правильная передача указателя
            { 
                perror("write");
            }
            putchar(c);
            fflush(stdout);
            nanosleep(&delay, NULL);
        }
        if (sem_post(sem)) 
        {
            perror("sem_post");
            break;
        }
        FD_ZERO(&readfds);
        FD_SET(STDIN_FILENO, &readfds);
        struct timespec timeout = {0, 0};
        if (pselect(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout, NULL) > 0) 
        {
            if (FD_ISSET(STDIN_FILENO, &readfds)) 
            {
                while (getchar() != '\n');
                break;
            }
        }
        nanosleep(&delay, NULL);
    }
    close(fd);
    sem_close(sem);
    sem_unlink(SEM_NAME);
    printf("\nProgram 2 complete\n");
    return EXIT_SUCCESS;
}
/*          Конец            */