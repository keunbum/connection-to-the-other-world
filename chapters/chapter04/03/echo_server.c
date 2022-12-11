#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define forn(i, N) for (int i = 0; i < (int)(N); ++i)

#define my_func(func, res, ...)                               \
    do                                                        \
    {                                                         \
        if ((res = func(__VA_ARGS__)) == -1)                  \
        {                                                     \
            error_handling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                     \
    } while (0)

#define my_socket(res, ...) my_func(socket, res, __VA_ARGS__)
#define my_bind(res, ...) my_func(bind, res, __VA_ARGS__)
#define my_listen(res, ...) my_func(listen, res, __VA_ARGS__)
#define my_accept(res, ...) my_func(accept, res, __VA_ARGS__)
#define my_write(res, ...) my_func(write, res, __VA_ARGS__)
#define my_close(res, ...) my_func(close, res, __VA_ARGS__)

#define set_addr(addr, domain, ip, port)        \
    do                                          \
    {                                           \
        memset(&addr, 0, sizeof(addr));         \
        serv_addr.sin_family = domain;          \
        serv_addr.sin_addr.s_addr = htonl(ip);  \
        serv_addr.sin_port = htons(atoi(port)); \
    } while (0);

void error_handling(const char *);

int main(int argc, char *argv[])
{
    int res;

    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len;

    char message[] = "Hello World!";

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    my_socket(serv_sock, PF_INET, SOCK_STREAM, 0);

    set_addr(serv_addr, AF_INET, INADDR_ANY, argv[1]);
    my_bind(res, serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    my_listen(res, serv_sock, 5);

    forn(_, 5)
    {
        clnt_addr_len = sizeof(clnt_addr);
        my_accept(res, serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        my_write(res, clnt_sock, message, sizeof(message));
        my_close(res, clnt_sock);
    }
    my_close(res, serv_sock);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}
