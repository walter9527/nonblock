#include "common.h"

void is_err(ssize_t ret, char *opt) {
    if (ret == -1) {
        perror(strcat(opt, " err"));
        exit(-1);
    }
}

void is_null_err(void *ret, char *opt) {
    if (ret == NULL) {
        perror(strcat(opt, " err"));
        exit(-1);
    }
}

void print_err(char *msg, int line, int err_no)
{
    printf("%d,%s:%s\n", line, msg, strerror(err_no));
    exit(-1);
}
