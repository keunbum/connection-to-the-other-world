#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>

int main(void)
{
    int fds[3];

    fds[0] = socket(PF_INET, SOCK_STREAM, 0);
    fds[1] = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC);
    fds[2] = socket(PF_INET, SOCK_DGRAM, 0);

    for (int i = 0; i < 3; i++)
    {
        printf("file descriptor %d: %d\n", i, fds[i]);
        close(fds[i]);
    }
    return 0;
}