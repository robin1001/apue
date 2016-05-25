#include <stdio.h>
//#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>

static void error_exit(char *msg) {
    perror(msg);
    //exit(1);
}

static int system(const char *cmd_string)
{
    pid_t pid;
    int status;
    if (cmd_string == NULL) return 1;
    if ((pid = fork()) < 0) {
        status = -1;
    }
    else if (pid == 0) {
        execl("/bin/sh", "sh", "-c", cmd_string, (char *)0);
        _exit(127);
    }
    else {
        while(waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                status = -1;
                break;
            }
        }
    }

    return status;
}


int main() {
    int status;
    if ((status = system("date")) < 0) error_exit("data error");

    return 0;
}

