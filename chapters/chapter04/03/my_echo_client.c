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
        memset(&addr, 0, sizeof(addr));       \
        addr.sin_family = domain;             \
        addr.sin_addr.s_addr = inet_addr(ip); \
        addr.sin_port = htons(atoi(port));    \
    } while (0);

#define BUF_SIZE (64)
#define TERMINATE_MSG_1 "Q\n"
#define TERMINATE_MSG_2 "q\n"

void error_handling(const char *);

int main(int argc, char *argv[])
{
    int res;
    int sock;
    struct sockaddr_in serv_addr;
    static char msg_buf[BUF_SIZE];

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

        fputs("Please enter the message you want to send(Q to quit): ", stdout);
        fgets(msg_buf, BUF_SIZE, stdin);
        if (strcmp(msg_buf, TERMINATE_MSG_1) == 0 || strcmp(msg_buf, TERMINATE_MSG_2) == 0)
        {
            break;
        }

        str_len = strlen(msg_buf);
        my_write(res, sock, msg_buf, str_len);

        total_read_bytes = 0;
        while (total_read_bytes < str_len)
        {
            my_read(read_bytes, sock, msg_buf + total_read_bytes, BUF_SIZE - 1);
            total_read_bytes += read_bytes;
        }
        msg_buf[total_read_bytes] = '\0';
        printf("Message from server: %s", msg_buf);
    }

    close(sock);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}
