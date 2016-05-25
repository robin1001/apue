#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>

static void sig_child(int signo);

int main()
{
    pid_t pid;
    signal(SIGCHLD, sig_child);
    pid = fork();
    if (pid < 0) {
        perror("fork error:");
        exit(1);
    }
    else if (pid == 0) {
        printf("I am child process,pid id %d.I am exiting.\n", getpid());
        exit(0);
    }
    printf("I am father process.I will sleep two seconds\n");

    sleep(2);
    system("ps -o pid,ppid,state,tty,command");
    printf("father process is exiting.\n");
    return 0;
}

static void sig_child(int signo)
{
    pid_t pid;
    int stat;
    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated.\n", pid);
}


