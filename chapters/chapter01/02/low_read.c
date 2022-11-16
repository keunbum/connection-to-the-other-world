#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#define BUF_SIZE 128

void error_handling(const char *);
void my_close(int);

int main(void)
{
    int fd;
    char buf[BUF_SIZE];

    fd = open("./data.txt", O_RDONLY);
    if (fd == -1)
    {
        my_close(fd);
        error_handling("open() error!");
    }
    printf("file descriptor: %d\n", fd);
    if (read(fd, buf, BUF_SIZE) == -1)
    {
        my_close(fd);
        error_handling("read() error!");
    }
    printf("file data: %s", buf);
    printf("strlen(buf): %ld\n", strlen(buf));
    
    my_close(fd);
    return 0;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

void my_close(int fd)
{
    if (close(fd) == -1)
    {
        error_handling("close() error!)");
    }
}