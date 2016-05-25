/* Created on 2016-04-20
 * Author: Binbin Zhang
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

int main()
{
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        perror("fork error:");
        exit(1);
    }
    if (pid == 0) {
        printf("I am the child process.\n");
        printf("pid: %d\tppid:%d\n", getpid(), getppid());
        printf("I will sleep five seconds.\n");
        // Wait parent exit
        sleep(5);
        printf("pid: %d\tppid:%d\n", getpid(), getppid());
        printf("child process is exited.\n");
    } 
    else {
        printf("I am father process.\n");
        // Wait child process output ppid
        sleep(1);
        printf("father process is  exited.\n");
    }
    return 0;
}

