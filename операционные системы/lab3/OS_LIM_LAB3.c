/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <mntent.h>
/*          Флаги процессов          */
volatile int FlagThread1 = 0;
volatile int FlagThread2 = 0;
int pipe_fd[2];
/*          Функция процесса 1 (запись)           */
void* ThreadProc1(void* arg) 
{
    FILE *fp = setmntent("/etc/mtab", "r");                                 // Открываем файл для чтения с информацией о файловых системах
    if (!fp) 
    {
        perror("setmntent");
        exit(EXIT_FAILURE);
    }
    struct mntent *mnt;                                                     // Структура для хранения информации 
    char Buff[256];                                                         // Буфер 
    while (!FlagThread1) 
    { 
        mnt = getmntent(fp);
        if (!mnt)                                                           // Если достигнут конец файла
        {
            endmntent(fp);                                                  // Закрываем текущий дескриптор
            fp = setmntent("/etc/mtab", "r");                               // Переоткрываем файл для чтения с начала
            if (!fp) 
            {
                perror("setmntent");
                break;
            }
            continue;
        }
                                                                            // Формируем строку для отправки: устройство + точка монтирования
        snprintf(Buff, sizeof(Buff), "%s %s\n", mnt->mnt_fsname, mnt->mnt_dir);
        ssize_t BytWritten;                                                 // Цикл записи с обработкой блокировок
        do 
        {
            BytWritten = write(pipe_fd[1], Buff, strlen(Buff));
            if (BytWritten == -1) 
            {
                perror("write");
                if (errno == EPIPE) break;                                  // Канал закрыт получателем
                if (errno == EAGAIN) sleep(1);
            }
        } 
        while (BytWritten == -1 && (errno == EAGAIN || errno == EWOULDBLOCK));
        sleep(1);
    }

    endmntent(fp);                                                          // Закрываем файл при завершении
    return NULL;
}
/*          Функция процесса 2 (чтение)           */
void* ThreadProc2(void* arg) 
{
    char Buff[256];                                                         // Буфер
    while (!FlagThread2) 
    {
        memset(Buff, 0, sizeof(Buff));                                      // Очищаем буфер перед чтением
        ssize_t BytRead = read(pipe_fd[0], Buff, sizeof(Buff) - 1);         // Пытаемся прочитать данные из канала
        if (BytRead == -1)                                                  // Обработка ошибок чтения
        {
            if (errno == EAGAIN || errno == EWOULDBLOCK) 
            {
                sleep(1);
                continue;
            } 
            else 
            {
                perror("read");
                break;
            }
        } 
        else if (BytRead == 0)                                              // Канал закрыт
        { 
            break;
        } 
        else 
        { 
            printf("Received: %s", Buff);                                   // Успешное чтение
        }
    }

    return NULL;
}
/*          Основной поток           */
int main(int argc, char *argv[]) 
{
    if (argc != 2)                                                          // Проверка аргументов командной строки (из скрипта)
    {
        fprintf(stderr, "Usage: %s <1|2|3>\n", argv[0]);
        return EXIT_FAILURE;
    }
    signal(SIGPIPE, SIG_IGN);                                               // Игнорируем SIGPIPE чтобы не падать при записи в закрытый канал
    if (strcmp(argv[1], "1") == 0)                                          // 1 - блокирующий pipe
    {
        if (pipe(pipe_fd) == -1) 
        {
            perror("pipe");
            return EXIT_FAILURE;
        }
    } 
    else if (strcmp(argv[1], "2") == 0)                                     // 2 - неблокирующий pipe
    {
        if (pipe(pipe_fd) == -1)                                            // Обычный pipe()
        {
            perror("pipe");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < 2; i++)                                         // Вручную ставим O_NONBLOCK
        {
            int Flags = fcntl(pipe_fd[i], F_GETFL);
            fcntl(pipe_fd[i], F_SETFL, Flags | O_NONBLOCK);
        }
    } else if (strcmp(argv[1], "3") == 0)                                   // 3 - неблокирующий pipe + fcntl
    {
        if (pipe(pipe_fd) == -1) 
        {
            perror("pipe");
            return EXIT_FAILURE;
        }
        for (int i = 0; i < 2; i++)                                         // Устанавливаем неблокирующий режим fcntl
        {
            int Flags = fcntl(pipe_fd[i], F_GETFL);
            if (Flags == -1) 
            {
                perror("fcntl");
                return EXIT_FAILURE;
            }
            if (fcntl(pipe_fd[i], F_SETFL, Flags | O_NONBLOCK) == -1) 
            {
                perror("fcntl");
                return EXIT_FAILURE;
            }
        }
    } 
    pthread_t Thread1, Thread2;
    pthread_create(&Thread1, NULL, ThreadProc1, NULL);                      // Создание потоков
    pthread_create(&Thread2, NULL, ThreadProc2, NULL);
    getchar();                                                              // Ожидание нажатия клавиши
    FlagThread1 = 1;                                                        // Устанавливаем флаги завершения
    FlagThread2 = 1;
    close(pipe_fd[0]);                                                      // Закрываем дескрипторы чтобы разблокировать потоки
    close(pipe_fd[1]);
    pthread_join(Thread1, NULL);                                            // Ожидание завершения потоков
    pthread_join(Thread2, NULL);
    return EXIT_SUCCESS;
}
/*          Конец            */