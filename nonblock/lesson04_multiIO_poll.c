#include "common.h"

int main(int argc, char *argv[])
{
    int ret;

    unsigned char data[3];
    int left, middle, right;
    signed char x, y;

    char buf[100];


    int mousefd = open("/dev/input/mouse1", O_RDWR);
    if (mousefd == -1) print_err("open mouse1 fail", __LINE__, errno);

    struct pollfd fds[2] = {
        {0, POLLIN},
        {mousefd, POLLIN}
//        {.fd = 0, .events = POLLIN},
//        {.fd = mousefd, .events = POLLIN}
    };

    while (1) {

label:        
        ret = poll(fds, 2, 5000);
        if (ret == -1 && errno == EINTR) goto label;
        else if (ret == -1) print_err("poll fail", __LINE__, errno);
        else if (ret > 0) {
            if (fds[0].events == fds[0].revents) {
                memset(buf, 0, sizeof(buf));
                if (read(fds[0].fd, buf, sizeof(buf)) > 0) {
                    printf("sever ==> %s", buf);
                }
            }

            if (fds[1].events == fds[1].revents) {
                memset(data, 0, sizeof(data));
                if (read(fds[1].fd, data, sizeof(data)) > 0) {
                    left = data[0] & 0x1;
                    right = data[0] & 0x2;
                    middle = data[0] & 0x4;

                    x = data[1];
                    y = data[2];
                    printf("x=%d, y=%d, left=%d, middle=%d, right=%d\n", x, y, left, middle, right);
                }
            }
        } else if (ret == 0) {
            printf("time out!!!\n");
        }
    }
    return 0;
}
