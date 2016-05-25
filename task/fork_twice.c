#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void error_exit(char *msg) {
    perror(msg);
    exit(1);
}


int main() 
{
    pid_t pid;
    if (pid = fork() < 0) {
        error_exit("fork error");
    }
    else if (pid == 0) {
        if ((pid = fork()) < 0) error_exit("fork error");
        else if (pid > 0) exit(0);

        sleep(2);
        printf("second child, pid %d ppid %d\n", getpid(), getppid());
        exit(0);
    }
    if (waitpid(pid, NULL, 0) != pid) {
        error_exit("waitpid error");
    }
    return 0;
}

