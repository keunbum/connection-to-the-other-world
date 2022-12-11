#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)

#define eprintf(...) fprintf(stderr, __VA_ARGS__)

#define my_func(func, res, ...)                               \
    do                                                        \
    {                                                         \
        if ((res = func(__VA_ARGS__)) == -1)                  \
        {                                                     \
            error_handling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                     \
    } while (0)

#define my_socket(res, ...) my_func(socket, res, __VA_ARGS__)
#define my_connect(res, ...) my_func(connect, res, __VA_ARGS__)
#define my_read(res, ...) my_func(read, res, __VA_ARGS__)
#define my_write(res, ...) my_func(write, res, __VA_ARGS__)
#define my_close(res, ...) my_func(close, res, __VA_ARGS__)

#define set_addr(addr, domain, ip, port)           \
    do                                             \
    {                                              \
        memset(&serv_addr, 0, sizeof(addr));       \
        serv_addr.sin_family = domain;             \
        serv_addr.sin_addr.s_addr = inet_addr(ip); \
        serv_addr.sin_port = htons(atoi(port));    \
    } while (0);

#define BUF_SIZE (64)
#define TERMINATE_MSG "Q\n"

void error_handling(const char *);

int main(int argc, char *argv[])
{
    int res;
    int sock;
    struct sockaddr_in serv_addr;
    static char msg_buf[BUF_SIZE];
    static char line[BUF_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    my_socket(sock, PF_INET, SOCK_STREAM, 0);

    set_addr(serv_addr, AF_INET, argv[1], argv[2]);

    my_connect(res, sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1)
    {
        int str_len;
        int read_bytes;
        int total_read_bytes;

        fputs("Please enter the message you want to send(type Q to exit): ", stdout);
        if (strcmp(fgets(line, BUF_SIZE, stdin), TERMINATE_MSG) == 0)
        {
            my_write(res, sock, "", 0);
            break;
        }

        str_len = strlen(line);
        my_write(res, sock, line, str_len + 1);

        memset(line, 0, sizeof(line));
        total_read_bytes = 0;
        while (total_read_bytes < str_len + 1)
        {
            my_read(read_bytes, sock, msg_buf, BUF_SIZE - 1);
            total_read_bytes += read_bytes;
            strncat(line, msg_buf, read_bytes);
        }
        printf("Message from server: %s", line);
    }

    close(sock);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}
