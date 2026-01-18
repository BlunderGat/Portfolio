/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <mqueue.h>
#include <mntent.h>
#include <unistd.h>
#include <fcntl.h>
/*          Флаг выхода          */
volatile int ExitFlag = 0;
mqd_t mqid;
/*          Функция процесса отправки           */
void* ThreadFunc(void* arg) 
{
    struct mq_attr attr;
    mq_getattr(mqid, &attr);
    char buffer[attr.mq_msgsize];
    while (!ExitFlag) 
    {                                                                               // Основной цикл обработки
        FILE *fp = setmntent("/proc/mounts", "r");                                  // Получаем список точек монтирования
        if (!fp) 
        {
            perror("setmntent");
            continue;
        }
        struct mntent *mnt;                                                         // Парсим каждую запись о монтировании
        while ((mnt = getmntent(fp)) != NULL) 
        {
            snprintf(buffer, sizeof(buffer),                                        // Формируем строку с информацией
                "%s %s %s %s",
                mnt->mnt_fsname, mnt->mnt_dir,
                mnt->mnt_type, mnt->mnt_opts);

            if (mq_send(mqid, buffer, strlen(buffer)+1, 0) == -1)                   // Отправляем сообщение в очередь
            {
                perror("mq_send");
            }
            printf("Отправлен: %s\n", buffer);
        }
        endmntent(fp);
        sleep(1);
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
    mqid = mq_open("/mnt_queue", O_CREAT | O_WRONLY | O_NONBLOCK, 0666, &attr);     // Создание/открытие очереди сообщений
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