#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();
    if (pid == 0)
    {
        sleep(15);
        return 24;
    }
    int status;
    int res;
    while ((res = waitpid(pid, &status, WNOHANG)) == 0)
    {
        sleep(3);
        puts("sleep 3sec.");
    }
    assert(res != -1);
    if (WIFEXITED(status))
    {
        printf("Child send %d\n", WEXITSTATUS(status));
    }
    return 0;
}