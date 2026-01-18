/*          Библиотеки           */
#include <ev.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
ev_child child_watcher;                                                                 // Объявляем наблюдатель за событием завершения дочернего процесса
/*          Функция обработчик события ev_child           */ 
static void child_cb(EV_P_ ev_child *w, int revents) 
{
    int status = w->rstatus;                                                            // код завершения, возвращенный waitpid
    if (WIFEXITED(status))                                                              // Если процесс завершился нормально, извлекаем код выхода
    {
        int exit_status = WEXITSTATUS(status);
        printf("Дочерний процесс (PID = %d) завершился с кодом %d\n", w->rpid, exit_status);
    } 
    else if (WIFSIGNALED(status)) 
    {
        printf("Дочерний процесс (PID = %d) завершился сигналом %d\n", w->rpid, WTERMSIG(status));
    } 
    else 
    {
        printf("Дочерний процесс (PID = %d) завершился неопределённым способом.\n", w->rpid);
    }
    ev_child_stop(EV_A_ w);                                                             // Останавливаем наблюдение и завершаем цикл событий
    ev_break(EV_A_ EVBREAK_ALL);
}
/*          Основной поток           */
int main(void) 
{
    pid_t pid = fork();
    if (pid < 0) 
    {
        perror("Ошибка fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0)                                                                // Дочерний процесс: замена процесса с помощью exec.
    {
        execlp("ls", "ls", NULL);                                                       // Выполняем команду "ls"
        perror("Ошибка execlp");
        exit(EXIT_FAILURE);
    } 
    else                                                                                // Родительский процесс:
    {
        struct ev_loop *loop = EV_DEFAULT;                                              // Создаем цикл ожидания и обработки событий по умолчанию и инициализируем наблюдатель за дочерним процессом
        ev_child_init(&child_watcher, child_cb, pid, 0);                                // Передаем в ev_child_init: адрес наблюдателя, callback, PID дочернего и флаг trace = 0
        ev_child_start(loop, &child_watcher);                                           // Регистрируем наблюдатель в цикле событий
        ev_run(loop, 0);                                                                // Запускаем цикл событий
    }
    return EXIT_SUCCESS;
}
/*          Конец            */