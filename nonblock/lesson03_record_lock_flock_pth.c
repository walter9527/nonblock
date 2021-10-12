#include "common.h"

struct global_val {
    sem_t sem[2];
} val;

void init() {
    sem_init(&val.sem[0], 0, 0);
    sem_init(&val.sem[1], 0, 1);
}

void syn() {
    if (sem_trywait(&val.sem[1]) == 0) {
        sem_wait(&val.sem[0]);
    } else {
        sem_post(&val.sem[1]);
        sem_post(&val.sem[0]);
    }
}

void* pth_fun(void *pth_arg)
{
    int fd = -1;
    int cnt = 100000;

    // 1. 各进程必须独自 open 
    // 2. 必须加 O_APPEND，否者会出现相互覆盖的情况
    fd = open("./file", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
    if (fd == -1) print_err("open ./file fail", __LINE__, errno);

    syn();

    while (cnt--) {
        flock(fd, LOCK_EX);
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        flock(fd, LOCK_UN);
    }

    return NULL;
}


// 多线程情况下的记录锁 -- flock 实现
int main(int argc, char *argv[])
{
    init();

    int fd = -1;
    int ret = 0;
    int cnt = 100000;
    pthread_t tid;

    ret = pthread_create(&tid, NULL, pth_fun, NULL);
    if (ret != 0) print_err("pthread create fail", __LINE__, errno);


    fd = open("./file", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
    if (fd == -1) print_err("open ./file fail", __LINE__, errno);

    syn();

    while (cnt--) {
        flock(fd, LOCK_EX);
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        flock(fd, LOCK_UN);
    }

    pthread_join(tid, NULL);
    return 0;
}
