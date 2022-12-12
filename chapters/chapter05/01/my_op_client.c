#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
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

#define set_addr(addr, domain, ip, port)      \
    do                                        \
    {                                         \
        memset(&addr, 0, sizeof(addr));       \
        addr.sin_family = domain;             \
        addr.sin_addr.s_addr = inet_addr(ip); \
        addr.sin_port = htons(atoi(port));    \
    } while (0);

#define BUF_SIZE (1024)
#define TERMINATE_MSG_1 "Q\n"
#define TERMINATE_MSG_2 "q\n"
#define OPS "+-*"

void error_handling(const char *);
int get_user_input(char *, int *);
int get_result_from_server(int, char *, int, int *);
int read_buf(char *, int, int *);
void make_final_result(char *, const char *, int, int);

int main(int argc, char *argv[])
{
    int res;
    int sock;
    struct sockaddr_in serv_addr;

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
        static char input_buf[BUF_SIZE];
        static char msg_buf[BUF_SIZE];

        if (!get_user_input(input_buf, &str_len))
        {
            break;
        }

        my_write(res, sock, input_buf, str_len);

        if (!get_result_from_server(sock, msg_buf, BUF_SIZE - 1, &res))
        {
            break;
        }
        make_final_result(msg_buf, input_buf, str_len, res);
        puts(msg_buf);
    }

    my_close(res, sock);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}

int get_user_input(char *input_buf, int *input_len)
{
    /*
    fputs("시작(1) 종료(0): ", stdout);
    fgets(input_buf, BUF_SIZE, stdin);
    if (input_buf[0] == '0')
    {
        return 0;
    }
    */

    fputs("숫자들을 입력하세요(q to quit): ", stdout);
    fgets(input_buf, BUF_SIZE, stdin);
    if (strchr("qQ", input_buf[0]))
    {
        return 0;
    }
    *input_len = strlen(input_buf);
    input_buf[*input_len - 1] = ' ';

    fputs("연산자를 입력하세요(q to quit): ", stdout);
    fgets(input_buf + *input_len, BUF_SIZE, stdin);
    if (strchr("qQ", input_buf[0]))
    {
        return 0;
    }
    *input_len += 1;
    input_buf[*input_len] = '\0';
    // eprintf("input_buf: %s, len: %d\n", input_buf, *input_len);
    return 1;
}

int get_result_from_server(int sockfd, char *buf, int nbytes, int *res)
{
    int read_bytes;
    while (1)
    {
        my_read(read_bytes, sockfd, buf, nbytes);
        if (read_bytes == 0)
        {
            return 0;
        }
        if (read_buf(buf, read_bytes, res))
        {
            break;
        }
    }
    return 1;
}

int read_buf(char *buf, int n, int *val)
{
    static int ing = 0;
    int sign = 1;
    forn(i, n)
    {
        if (buf[i] == ' ')
        {
            *val = ing * sign;
            ing = 0;
            sign = 1;
            assert(i == n - 1);
            return 1;
        }
        if (buf[i] == '-')
        {
            sign *= -1;
            continue;
        }
        ing = ing * 10 + (int)(buf[i] - '0');
    }
    return 0;
}

void make_final_result(char *des, const char *src, int n, int result_val)
{
    int ptr = 0;
    forn(i, n - 1)
    {
        if (src[i] == ' ')
        {
            des[ptr++] = ' ';
            des[ptr++] = src[n - 1];
            des[ptr++] = ' ';
            continue;
        }
        des[ptr++] = src[i];
    }
    sprintf(des + ptr - 2, "= %d", result_val);
}
