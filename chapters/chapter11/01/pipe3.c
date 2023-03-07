#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_SIZE (64)

static void error_handling(const char*);

int main(int argc, char* argv[])
{
    int fds[2][2];
    pid_t pid;
    char msg1[] = "Hey!";
    char msg2[] = "Hi!";
    char buf[BUF_SIZE];
    for (int i = 0; i < 2; ++i)
    {
        if (pipe(fds[i]) == -1)
        {
            error_handling("pipe() error");
        }        
    }
    pid = fork();
    if (pid == 0)
    {
        write(fds[0][1], msg1, sizeof(msg1));
        read(fds[1][0], buf, sizeof(buf));
        printf("Child proc output: %s\n", buf);
        return 0;
    }
    read(fds[0][0], buf, BUF_SIZE);
    printf("Parent proc output: %s\n", buf);
    write(fds[1][1], msg2, sizeof(msg2));
    sleep(1);
    return 0;
}

static void error_handling(const char* message)
{
	perror(message);
	exit(1);
}