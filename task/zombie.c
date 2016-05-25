#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main() 
{
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork error");
    }
    else if (pid == 0) {
        printf("I'm child process %d, exiting\n", getpid());
        exit(0);
    }
    else {
        printf("parent process, sleep 3s\n");
        while (1) {
            sleep(1);
        }
    }
    return 0;
}



