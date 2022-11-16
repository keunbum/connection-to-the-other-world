#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(const char *, int);
void my_close(int);

int main(void)
{
    int fd;
    char buf[] = "Let's go!\n";

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        error_handling("open() error!", fd);
    }
    printf("file descriptor: %d\n", fd);
    printf("sizeof(buf) = %ld\n", sizeof(buf));
    printf("strlen(buf) = %ld\n", strlen(buf));

    if (write(fd, buf, strlen(buf)) == -1)
    {
        error_handling("write() error!", fd);
    }

    my_close(fd);
    return 0;
}

void error_handling(const char *message, int fd)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    my_close(fd);
    exit(1);
}

void my_close(int fd)
{
    if (close(fd) == -1) {
        error_handling("close() error!)", fd);
    }
}