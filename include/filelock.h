#ifndef H_FILELOCK_H
#define H_FILELOCK_H

#include <fcntl.h>

#ifndef __USE_FILE_OFFSET64
    typedef __off_t my_off_t;
#else
    typedef __off64_t my_off_t;
#endif


// 非阻塞写锁
#define SET_WRFLCK(fd, l_whence, l_offset, l_len) \
        set_filelock(fd, F_SETLK, F_WRLCK, l_whence, l_offset, l_len)

// 阻塞写锁
#define SET_WRFLCK_W(fd, l_whence, l_offset, l_len) \
        set_filelock(fd, F_SETLKW, F_WRLCK, l_whence, l_offset, l_len)

// 非阻塞读锁
#define SET_RDFLCK(fd, l_whence, l_offset, l_len) \
        set_filelock(fd, F_SETLK, F_RDLCK, l_whence, l_offset, l_len)

// 阻塞读锁
#define SET_RDFLCK_W(fd, l_whence, l_offset, l_len) \
        set_filelock(fd, F_SETLKW, F_RDLCK, l_whence, l_offset, l_len)

// 解锁
#define UNLCK(fd, l_whence, l_offset, l_len) \
        set_filelock(fd, F_SETLK, F_UNLCK, l_whence, l_offset, l_len)


/**
 *  阻塞/非阻塞 加读锁和写锁
 *
 */
static void set_filelock(int fd, int ifwait, short int l_type, short int l_whence, my_off_t l_start, my_off_t l_len)
{
    struct flock flck;

    flck.l_type = l_type; 
    flck.l_whence = l_whence;
    flck.l_start = l_start;
    flck.l_len = l_len;

    if (fcntl(fd, ifwait, &flck) == -1) {
        perror("fcmtl fail");
        exit(-1);
    }
}

#endif
