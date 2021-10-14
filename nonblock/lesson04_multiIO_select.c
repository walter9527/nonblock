#include "common.h"

int main(int argc, char *argv[])
{
    int ret;

    unsigned char data[3];
    int left, middle, right;
    signed char x, y;

    char buf[100];

    struct timeval tv = {5, 500000};

    fd_set readfds;
    int mousefd = open("/dev/input/mouse1", O_RDWR);
    if (mousefd == -1) print_err("open mouse1 fail", __LINE__, errno);

    while (1) {

        // 将 0 和 mousefd 加入读集合
       FD_ZERO(&readfds);
       FD_SET(mousefd, &readfds);
       FD_SET(0, &readfds);

label: ret = select(mousefd + 1, &readfds, NULL, NULL, &tv);
       if (ret == -1 && errno == EINTR) goto label; // select 被信号唤醒，重新 select
       else if (ret == -1) print_err("select fail", __LINE__, errno); // select 出错
       else if (ret > 0) { // select 正常工作，读取数据
           if (FD_ISSET(0, &readfds)) {
               memset(buf, 0, sizeof(buf));
               if (read(0, buf, sizeof(buf)) > 0) {
                   printf("sever ==> %s", buf);
               }
           }

           if (FD_ISSET(mousefd, &readfds)) {
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
       } else if (ret == 0) { // 超时
           printf("time out!!!\n");
       }

    }

    return 0;
}
