#include "common.h"
#include "filelock.h"

int main(int argc, char *argv[])
{
    int fd = 0;
    int ret = 0;

    fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) print_err("open ./file fail", __LINE__, errno);


    ret = fork();

    if (ret > 0) { // 父进程
        while (1) {
            SET_RDFLCK_W(fd, SEEK_SET, 0, 0);
            write(fd, "hello ", 6);
            write(fd, "world\n", 6);
            UNLCK(fd, SEEK_SET, 0, 0);
        }
    } else if (ret == 0) { // 子进程
        while (1) {
            SET_RDFLCK_W(fd, SEEK_SET, 0, 0);
            write(fd, "hello ", 6);
            write(fd, "world\n", 6);
            UNLCK(fd, SEEK_SET, 0, 0);
        }
    }

    return 0;
}
