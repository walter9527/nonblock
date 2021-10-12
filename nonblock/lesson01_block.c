#include "common.h"

void print_err(char *msg, int line, int err_no)
{
    printf("%d,%s:%s\n", line, msg, strerror(err_no));
    exit(-1);
}

int main(int argc, char *argv[])
{
    int cor;
    int mousefd;
    int ret;
    char buf[100];
#if 0    
    mousefd = open("/dev/input/mouse2", O_RDONLY | O_NONBLOCK);
    if (mousefd == -1) print_err("open mouse0 fail", __LINE__, errno);
#endif

    int flag = fcntl(0, F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(0, F_SETFL, flag);

    ret = scanf("%s", buf);
    if (ret > 0) printf("%s\n", buf);
    perror("hello");

#if 0
    while (1) {
        ret = read(0, buf, sizeof(buf));
        if (ret > 0) printf("%s", buf);
        else if (ret == -1 && errno != EAGAIN) print_err("read input fail", __LINE__, errno);

        memset(buf, 0, sizeof(buf));
        ret = read(mousefd, &cor, sizeof(cor));
        if (ret > 0) printf("%d\n", cor);
        else if (ret == -1 && errno != EAGAIN) print_err("read mouse0 fail", __LINE__, errno);

    }
#endif
}
