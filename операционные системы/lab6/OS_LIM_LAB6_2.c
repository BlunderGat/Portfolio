/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#define SHM_SIZE 256
#define SHM_KEY_FILE "/etc/fstab"                                       // Используем стабильный путь для получения ключа
#define SHM_PROJ_ID 'M'
#define SEM_WRITE_NAME "/sem_write_example"
#define SEM_READ_NAME  "/sem_read_example"
volatile sig_atomic_t TerminateFlag = 0;
sem_t *sem_write = NULL;
sem_t *sem_read  = NULL;
void SigintHandler(int signum)
{
    TerminateFlag = 1;
    sem_post(sem_write);                                                // Разблокируем sem_wait в потоке, если он ожидает
}
/*          Функция потока получения          */
void *ReceiverThread(void *arg)
{
    int shmid = *(int *)arg;
    char *shm_ptr = shmat(shmid, NULL, 0);                              // Присоединение разделяемой памяти
    if (shm_ptr == (void *)-1)
    {
        perror("shmat");
        pthread_exit(NULL);
    }
    char local_buffer[SHM_SIZE];
    while (!TerminateFlag)
    {
        if (sem_wait(sem_write) == -1)                                  // Ожидаем, пока отправитель положит данные
        {
            if (errno == EINTR && TerminateFlag)
                break;
            perror("sem_wait (sem_write)");
            break;
        }
        memcpy(local_buffer, shm_ptr, SHM_SIZE);
        local_buffer[SHM_SIZE - 1] = '\0';
        printf("Получено: %s\n", local_buffer);
        fflush(stdout);
        if (sem_post(sem_read) == -1)                                   // Сигнализируем отправителю, что данные прочитаны
        {
            perror("sem_post (sem_read)");
            break;
        }
    }
    shmdt(shm_ptr);
    pthread_exit(NULL);
}
/*          Основной поток           */
int main(void)
{
    struct sigaction sa;                                                // Настраиваем обработчик SIGINT через sigaction
    sa.sa_handler = SigintHandler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;                                                    // Без SA_RESTART
    sigaction(SIGINT, &sa, NULL);
    key_t key = ftok(SHM_KEY_FILE, SHM_PROJ_ID);
    if (key == -1)
    {
        perror("ftok");
        exit(EXIT_FAILURE);
    }
    int shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
    if (shmid == -1)
    {
        perror("shmget");
        exit(EXIT_FAILURE);
    }
    sem_write = sem_open(SEM_WRITE_NAME, O_CREAT, 0666, 0);             // Создаём именованные семафоры один раз в main
    if (sem_write == SEM_FAILED)
    {
        perror("sem_open (sem_write)");
        exit(EXIT_FAILURE);
    }
    sem_read = sem_open(SEM_READ_NAME, O_CREAT, 0666, 1);
    if (sem_read == SEM_FAILED)
    {
        perror("sem_open (sem_read)");
        exit(EXIT_FAILURE);
    }
    pthread_t Thread;
    if (pthread_create(&Thread, NULL, ReceiverThread, &shmid) != 0)
    {
        perror("pthread_create");
        exit(EXIT_FAILURE);
    }
    printf("Программа-получатель запущена. Нажмите ENTER для завершения...\n");
    fflush(stdout);
    getchar();
    TerminateFlag = 1;
    sem_post(sem_write);                                                // Если поток ждёт в sem_wait, разблокируем его
    pthread_join(Thread, NULL);
    sem_close(sem_write);                                               // Очистка ресурсов
    sem_close(sem_read);
    sem_unlink(SEM_WRITE_NAME);
    sem_unlink(SEM_READ_NAME);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
/*          Конец            */