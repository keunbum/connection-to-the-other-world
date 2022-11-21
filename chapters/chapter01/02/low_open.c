#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(const char *);
void my_close(int);

int main(void)
{
    int fd;
    char buf[] = "Let's go!\n";

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    if (fd == -1)
    {
        error_handling("open() error");
    }

    printf("file descriptor: %d\n", fd);
    printf("sizeof(buf) = %ld\n", sizeof(buf));
    printf("strlen(buf) = %ld\n", strlen(buf));

    if (write(fd, buf, sizeof(buf)) == -1)
    {
        my_close(fd);
        error_handling("write() error");
    }

    my_close(fd);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}

void my_close(int fd)
{
    if (close(fd) == -1)
    {
        error_handling("close() error");
    }
}