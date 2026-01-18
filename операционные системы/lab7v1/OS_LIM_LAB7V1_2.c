/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
/*          Флаг выхода          */
volatile int ExitFlag = 0;
mqd_t mqid;
/*          Функция процесса получения           */
void* ThreadFunc(void* arg) 
{
    struct mq_attr attr;
    mq_getattr(mqid, &attr);
    char buffer[attr.mq_msgsize];
    while (!ExitFlag)                                                               // Цикл обработки сообщений
    {
        ssize_t bytes = mq_receive(mqid, buffer, sizeof(buffer), NULL);             // Прием сообщения из очереди
        if (bytes > 0) {
            printf("Получен: %s\n", buffer);
        } 
        else 
        {
            int error_code = errno;                                                 // Обработка ошибок чтения
            if (error_code != EAGAIN) 
            {
                perror("mq_receive");
            }
        }
        usleep(100000);
    }
    return NULL;
}
/*          Основной поток           */
int main() 
{
    pthread_t tid;
    struct mq_attr attr = {                                                         // Настройка параметров очереди
        .mq_flags = O_NONBLOCK,
        .mq_maxmsg = 10,
        .mq_msgsize = 1024
    };
    mqid = mq_open("/mnt_queue", O_CREAT | O_RDONLY | O_NONBLOCK, 0666, &attr);     // Открытие очереди в режиме чтения
    if (mqid == (mqd_t)-1) 
    {
        perror("mq_open");
        exit(1);
    }
    pthread_create(&tid, NULL, ThreadFunc, NULL);
    printf("Нажмите Enter для завершения...\n");
    getchar();
    ExitFlag = 1;
    pthread_join(tid, NULL);
    mq_close(mqid);
    mq_unlink("/mnt_queue");
    return 0;
}
/*          Конец            */