/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <mntent.h>
#include <signal.h>
#include <stdatomic.h>
#include <pthread.h>
#include <sys/queue.h>
#define PORT 9090                                                                                                       // Порт для UDP-соединения
#define BUFFER_SIZE 1024                                                                                                // Максимальный размер буфера данных
atomic_int request_counter = 0;                                                                                         // Глобальные флаги
volatile sig_atomic_t running = 1;                                                                                      // Флаг работы программы
struct Request {                                                                                                        // Структура для хранения информации о запросе
    struct sockaddr_in addr;
    socklen_t addr_len;
    TAILQ_ENTRY(Request) entries;
};
TAILQ_HEAD(request_queue, Request);                                                                                     // Очередь запросов
struct request_queue request_queue;
pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;                                                                // Синхронизационные примитивы
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;
/*          Инициирует завершение работы сервера           */
void trigger_shutdown()                                                                                                 // Обработчик завершения
{
    running = 0;
    pthread_cond_broadcast(&queue_cond);                                                                                // Пробуждаем все потоки
}
/*          Обработчик сигналов           */
void handle_signal(int sig) 
{
    trigger_shutdown();
}
/*          Поток приема запросов           */
void* receiver_thread(void* arg) 
{
    int sockfd = *(int*)arg;
    while (running) 
    {
        struct Request *req = malloc(sizeof(struct Request));
        req->addr_len = sizeof(req->addr);
        ssize_t n = recvfrom(sockfd, NULL, 0, MSG_TRUNC|MSG_DONTWAIT, (struct sockaddr*)&req->addr, &req->addr_len);    // Неблокирующий прием
        if (n < 0) 
        {
            free(req);
            usleep(100000);
            continue;
        }
        pthread_mutex_lock(&queue_mutex);                                                                               // Добавление запроса в очередь
        TAILQ_INSERT_TAIL(&request_queue, req, entries);
        pthread_cond_signal(&queue_cond);
        pthread_mutex_unlock(&queue_mutex);
    }
    return NULL;
}
/*          Поток обработки запросов           */
void* processor_thread(void* arg) 
{
    int sockfd = *(int*)arg;
    while (running) 
    {
        pthread_mutex_lock(&queue_mutex);
        while (TAILQ_EMPTY(&request_queue)) 
        {
            pthread_cond_wait(&queue_cond, &queue_mutex);
            if (!running) break;
        }
        if (!running) 
        {
            pthread_mutex_unlock(&queue_mutex);
            break;
        }
        struct Request *req = TAILQ_FIRST(&request_queue);
        TAILQ_REMOVE(&request_queue, req, entries);
        pthread_mutex_unlock(&queue_mutex);
        int req_num = atomic_fetch_add(&request_counter, 1) + 1;                                                        // Обработка запроса
        FILE* fp = setmntent("/etc/fstab", "r");
        if (fp) 
        {
            struct mntent* mnt;
            while ((mnt = getmntent(fp)) != NULL) 
            {
                char response[BUFFER_SIZE];
                snprintf(response, sizeof(response), 
                    "[Запрос %d] %-15s %-20s %-10s %s",
                    req_num, mnt->mnt_fsname, mnt->mnt_dir, 
                    mnt->mnt_type, mnt->mnt_opts);
                sendto(sockfd, response, strlen(response), 0,
                     (struct sockaddr*)&req->addr, req->addr_len);
            }
            endmntent(fp);
            sendto(sockfd, "END", 3, 0, (struct sockaddr*)&req->addr, req->addr_len);
        }
        free(req);
    }
    return NULL;
}
/*          Основной поток           */
int main() 
{
    int sockfd;
    struct sockaddr_in server_addr;
    struct sigaction sa = { .sa_handler = handle_signal };                                                              // Настройка обработки сигналов
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    TAILQ_INIT(&request_queue);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("Ошибка создания сокета");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) 
    {
        perror("Ошибка привязки");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    pthread_t receiver, processor;
    pthread_create(&receiver, NULL, receiver_thread, &sockfd);
    pthread_create(&processor, NULL, processor_thread, &sockfd);
    printf("Сервер запущен. Нажмите Enter для выхода\n");
    int stdin_fd = fileno(stdin);
    while (running) 
    {
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(stdin_fd, &fds);
        struct timeval tv = {.tv_sec = 1};
        if (select(stdin_fd+1, &fds, NULL, NULL, &tv) > 0) 
        {
            if (getchar() == '\n') 
            {
                trigger_shutdown();
                break;
            }
        }
    }
    pthread_join(receiver, NULL);
    pthread_join(processor, NULL);
    close(sockfd);                                                                                                      // Корректное закрытие сокета
    printf("Сервер завершил работу\n");
    return 0;
}
/*          Конец            */