#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        return 3;
    }
    printf("Child pid: %d\n", pid);
    pid = fork();
    if (pid == 0)
    {
        exit(7);
    }
    printf("Child pid: %d\n", pid);
    for (int _ = 0; _ < 2; ++_)
    {
        int cstatus;
        pid = wait(&cstatus);
        if (WIFEXITED(cstatus))
        {
            printf("Child-%d send %d\n", pid, WEXITSTATUS(cstatus));
        }
    }
    //sleep(10);
    return 0;
}