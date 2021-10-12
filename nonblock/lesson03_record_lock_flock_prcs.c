#include "common.h"

// 多进程情况下的记录锁 -- flock 实现
int main(int argc, char *argv[])
{
    int fd = -1;
    int ret = -1;


    ret = fork();

    if (ret > 0) {
        
        // 1. 各进程必须独自 open 
        // 2. 必须加 O_APPEND，否者会出现相互覆盖的情况
        fd = open("./file", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
        if (fd == -1) print_err("open ./file fail", __LINE__, errno);

        while (1) {
            flock(fd, LOCK_EX);
            write(fd, "hello ", 6);
            write(fd, "world\n", 6);
            flock(fd, LOCK_UN);
        }

    } else if (ret == 0) {

        fd = open("./file", O_RDWR | O_CREAT | O_TRUNC | O_APPEND, 0644);
        if (fd == -1) print_err("open ./file fail", __LINE__, errno);

        while (1) {
            flock(fd, LOCK_EX);
            write(fd, "hello ", 6);
            write(fd, "world\n", 6);
            flock(fd, LOCK_UN);
        }
    }

    return 0;
}
