#include "common.h"
#include "filelock.h"

//  int fd = -1;

void *pth_fun(void *pth_arg)
{
    // 必须自己创建的 fd 才有效，否则文件锁没有效果
    int fd = -1;
    fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) print_err("open ./file fail", __LINE__, errno);

    while (1) {
        SET_WRFLCK_W(fd, SEEK_SET, 0, 0);
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        UNLCK(fd, SEEK_SET, 0, 0);
    }
    return NULL;
}

int main(int argc, char *argv[])
{
    int fd = -1;
    int ret = -1;
    pthread_t tid;

    fd = open("./file", O_RDWR | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) print_err("open ./file fail", __LINE__, errno);


    ret = pthread_create(&tid, NULL, pth_fun, NULL);
    if (ret == -1) print_err("pthread_create fail", __LINE__, ret);

    while (1) {
        SET_WRFLCK_W(fd, SEEK_SET, 0, 0);
        write(fd, "hello ", 6);
        write(fd, "world\n", 6);
        UNLCK(fd, SEEK_SET, 0, 0);
    }


    return 0;
}
