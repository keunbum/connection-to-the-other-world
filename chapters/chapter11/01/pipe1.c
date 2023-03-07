#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE (64)

static void error_handling(const char*);

int main(int argc, char* argv[])
{
    int fds[2];
    pid_t pid;
    char msg[] = "Hey!";
    char buf[BUF_SIZE];
    if (pipe(fds) == -1)
    {
        error_handling("pipe() error");
    }
    pid = fork();
    if (pid == 0)
    {
        write(fds[1], msg, sizeof(msg));
        return 0;
    }
    read(fds[0], buf, BUF_SIZE);
    printf("message from child-%d:%s\n", pid, buf);
    return 0;
}

static void error_handling(const char* message)
{
	perror(message);
	exit(1);
}