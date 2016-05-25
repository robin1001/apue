#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/types.h>
#include <sys/stat.h>

int main (void) {
    int fd = -1;

    fd = open("tmp", O_WRONLY | O_CREAT | O_TRUNC, 0664);
    /* if error */
#if 1
    close(1); // 关闭标准输出
    dup(fd);
#else 
    dup2(fd, 1);
    close(fd);
#endif
    puts("dup test.");

    return 0;
}
