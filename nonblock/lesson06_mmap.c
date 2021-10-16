#include "common.h"


int main(int argc, char *argv[])
{
    int srcfd = open("./fileA", O_RDONLY);
    if (srcfd == -1) print_err("open fileA fail", __LINE__, errno); 
    int dstfd = open("./fileB", O_RDWR | O_CREAT, 0644);
    if (dstfd == -1) print_err("open fileB fail", __LINE__, errno); 

    off_t size = lseek(srcfd, 0, SEEK_END);

    ftruncate(dstfd, size);

    void *srcfile = mmap(NULL, size, PROT_READ, MAP_PRIVATE, srcfd, 0);
    if (srcfile == MAP_FAILED) print_err("mmap fileA fail", __LINE__, errno); 
    
    void *dstfile = mmap(NULL, size, PROT_WRITE, MAP_SHARED, dstfd, 0);
    if (dstfile == MAP_FAILED) print_err("mmap fileB fail", __LINE__, errno); 

    memcpy(dstfile, srcfile, size);
    return 0;
}
