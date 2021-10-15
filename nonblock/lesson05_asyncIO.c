#include "common.h"

int mousefd;

unsigned char data[3];
int left, middle, right;
signed char x, y;

void sig_fun(int sig) {

    memset(data, 0, sizeof(data));
    if (read(mousefd, data, sizeof(data)) > 0) {
       left = data[0] & 0x1;
       right = data[0] & 0x2;
       middle = data[0] & 0x4;

       x = data[1];
       y = data[2];
       printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
    }
}

int main(int argc, char *argv[])
{
    char buf[100];

    mousefd = open("/dev/input/mouse1", O_RDWR);
    if (mousefd == -1) print_err("open mouse1 fail", __LINE__, errno);

    // 告诉鼠标驱动，他发送的SIGIO信号由当前进程接收
    fcntl(mousefd, F_SETOWN, getpid());

    // 对 mousefd 进行设置，让其支持异步IO
    int flag = fcntl(mousefd, F_GETFL);
    flag |= O_ASYNC;
    fcntl(mousefd, F_SETFL, flag);

    // SIGIO 设置捕获函数，在捕获函数里面读鼠标
    signal(SIGIO, &sig_fun);

    while (1) {
        memset(buf, 0, sizeof(buf));
        if (read(0, buf, sizeof(buf)) > 0) {
            printf("sever ==> %s", buf);
        }
    }

    return 0;
}
