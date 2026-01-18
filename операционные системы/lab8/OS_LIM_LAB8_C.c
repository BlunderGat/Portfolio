/*          Библиотеки           */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <signal.h>
#define SERVER_IP "127.0.0.1"                                                                       // IP сервера для локального подключения
#define PORT 9090                                                                                   // Порт сервера
#define BUFFER_SIZE 1024                                                                            // Размер буфера приема данных
volatile sig_atomic_t running = 1;                                                                  // Флаг работы программы
/*          Обработчик сигналов для корректного завершения           */
void handle_signal(int sig) 
{
    running = 0;
}
/*          Поток отправки запросов           */
void* sender_thread(void* arg)                                                                      // Обработчик сигналов для корректного завершения
{
    int sockfd = *(int*)arg;                                                                        // Дескриптор сокета
    struct sockaddr_in server_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(PORT),
        .sin_addr.s_addr = inet_addr(SERVER_IP)
    };
    while (running) 
    {
        sendto(sockfd, "GET", 3, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));           // Отправляем триггер
        sleep(1);
    }
    return NULL;
}
/*          Поток приема данных           */
void* receiver_thread(void* arg) 
{
    int sockfd = *(int*)arg;
    char buffer[BUFFER_SIZE];
    while (running) 
    {
        struct sockaddr_in server_addr;
        socklen_t len = sizeof(server_addr);
        fd_set fds;
        FD_ZERO(&fds);
        FD_SET(sockfd, &fds);
        struct timeval tv = {.tv_sec = 1};
        if (select(sockfd+1, &fds, NULL, NULL, &tv) <= 0) continue;
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &len);
        if (n <= 0) continue;
        buffer[n] = '\0';
        if (strcmp(buffer, "END") == 0) 
        {
            printf("\n------------------------------------------------------------------------------------------------------------\n");
        } 
        else 
        {
            printf("%s\n", buffer);
        }
    }
    return NULL;
}
/*          Основной поток           */
int main() 
{
    int sockfd;
    struct sigaction sa = { .sa_handler = handle_signal };
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    pthread_t sender, receiver;
    pthread_create(&sender, NULL, sender_thread, &sockfd);
    pthread_create(&receiver, NULL, receiver_thread, &sockfd);
    printf("Клиент запущен. Нажмите Enter для выхода\n");
    while (running && getchar() != '\n') {}
    running = 0;
    pthread_join(sender, NULL);
    pthread_join(receiver, NULL);
    close(sockfd);                                                                                  // Корректное закрытие сокета
    printf("Клиент завершил работу\n");
    return 0;
}
/*          Конец            */     