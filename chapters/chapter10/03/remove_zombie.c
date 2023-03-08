#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <sys/wait.h>

void read_childproc(int sig)
{
    int cstatus;
    pid_t pid = waitpid(-1, &cstatus, WNOHANG);
    assert(WIFEXITED(cstatus));
    printf("Removed proc id: %d\n", pid);
    printf("Child send %d\n", WEXITSTATUS(cstatus));
}

int main(int argc, char *argv[])
{
    struct sigaction act = {};
    {
        act.sa_handler = read_childproc;
        act.sa_flags = 0;
        sigaction(SIGCHLD, &act, 0);
    }
    {
        int pid = fork();
        if (pid == 0)
        {
            puts("Hi I'm Child.");
            sleep(10);
            return 12;
        }
        printf("Child proc id: %d\n", pid);
    }
    {
        int pid = fork();
        if (pid == 0)
        {
            puts("Hi I'm Child.");
            sleep(10);
            exit(24);
        }
        printf("Child proc id: %d\n", pid);
    }
    for (int _ = 0; _ < 5; ++_)
    {
        puts("wait...");
        sleep(5);
    }
    return 0;
}