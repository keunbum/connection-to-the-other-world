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
        eprintf(#func "(" #__VA_ARGS__ ")\n");                \
        if ((res = func(__VA_ARGS__)) == -1)                  \
        {                                                     \
            error_handling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                     \
    } while (0)

#define my_socket(res, ...) my_func(socket, res, __VA_ARGS__)
#define my_bind(res, ...) my_func(bind, res, __VA_ARGS__)
#define my_listen(res, ...) my_func(listen, res, __VA_ARGS__)
#define my_accept(res, ...) my_func(accept, res, __VA_ARGS__)
#define my_read(res, ...) my_func(read, res, __VA_ARGS__)
#define my_write(res, ...) my_func(write, res, __VA_ARGS__)
#define my_close(res, ...) my_func(close, res, __VA_ARGS__)

#define set_addr(addr, domain, ip, port)   \
    do                                     \
    {                                      \
        memset(&addr, 0, sizeof(addr));    \
        addr.sin_family = domain;          \
        addr.sin_addr.s_addr = htonl(ip);  \
        addr.sin_port = htons(atoi(port)); \
    } while (0);

#define my_debug_s(pref, s)                \
    do                               \
    {                                \
        eprintf(#pref ": " #s ": \"%s\"\n", s); \
    } while (0);

#define my_debug_a(pref, a, n, c)        \
    do                             \
    {                              \
        eprintf(#pref ": " #a "[" #n "]:");   \
        forn(i, n)                 \
        {                          \
            eprintf(" " #c, a[i]); \
        }                          \
        eprintf("\n");             \
    } while (0);

#define BUF_SIZE (1024)
#define REQUEST_QUEUE_SIZE (5)
#define CLIENT_ITER_NUM (5)
#define OPS "+-*"

void my_init(void);
void error_handling(const char *);
int get_result_from_client_input(int, char *, int, int *);
int read_buf(char *, int, char *);
int count_nums(const char *, int);
void handle_s(const char *, int, int *, char *);
int evaluate(const int *, int, int (*)(int *, int));
void add(int *, int);
void sub(int *, int);
void mul(int *, int);

static int ch2int[128];

int main(int argc, char *argv[])
{
    int tmp_res;
    int serv_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_len;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    my_init();

    my_socket(serv_sock, PF_INET, SOCK_STREAM, 0);

    set_addr(serv_addr, AF_INET, INADDR_ANY, argv[1]);
    my_bind(tmp_res, serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    my_listen(tmp_res, serv_sock, REQUEST_QUEUE_SIZE);

    clnt_addr_len = sizeof(clnt_addr);
    forn(i, CLIENT_ITER_NUM)
    {
        int result_value;
        int clnt_sock;
        static char msg_buf[BUF_SIZE];

        my_accept(clnt_sock, serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_len);
        eprintf("%dth client connected.\n", i + 1);

        while (get_result_from_client_input(clnt_sock, msg_buf, BUF_SIZE - 1, &result_value))
        {
            sprintf(msg_buf, "%d ", result_value);
            my_write(tmp_res, clnt_sock, msg_buf, strlen(msg_buf));
        }

        my_close(tmp_res, clnt_sock);
    }

    my_close(tmp_res, serv_sock);
    return 0;
}

void my_init(void)
{
    int ops_sz = strlen(OPS);
    forn(i, ops_sz)
    {
        ch2int[(int)OPS[i]] = i;
    }
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}

int get_result_from_client_input(int sockfd, char *buf, int nbytes, int *res)
{
    static const void *funcs[3] = {add, sub, mul};
    static char input[BUF_SIZE];
    int read_bytes;
    int *a;
    int n;
    int str_len;
    char op;

    while (1)
    {
        my_read(read_bytes, sockfd, buf, nbytes);
        if (read_bytes == 0)
        {
            return 0;
        }
        if ((str_len = read_buf(buf, read_bytes, input)))
        {
            n = count_nums(input, str_len);
            a = (int *)malloc(sizeof(int) * n);
            handle_s(input, str_len, a, &op);
            *res = evaluate(a, n, funcs[ch2int[(int)op]]);
            free(a);
            break;
        }
    }
    return 1;
}

int read_buf(char *buf, int n, char *res)
{
    static int str_len = 0;
    static char tmp_buf[BUF_SIZE];
    int ret = -1;

    forn(i, n)
    {
        tmp_buf[str_len++] = buf[i];
        if (strchr(OPS, buf[i]))
        {
            strncpy(res, tmp_buf, str_len);
            ret = str_len;
            str_len = 0;
            return ret;
        }
    }
    return 0;
}

int count_nums(const char *s, int len)
{
    int ret = 0;
    forn(i, len)
    {
        ret += (s[i] == ' ');
    }
    return ret;
}

void handle_s(const char *s, int len, int *a, char *op)
{
    int val = 0;
    int n = 0;
    forn(i, len - 1)
    {
        if (s[i] == ' ')
        {
            a[n++] = val;
            val = 0;
            continue;
        }
        val = val * 10 + (int)(s[i] - '0');
    }
    *op = s[len - 1];
}

int evaluate(const int *a, int n, int (*func)(int *, int))
{
    int ret = a[0];
    for (int i = 1; i < n; ++i)
    {
        func(&ret, a[i]);
    }
    return ret;
}

void add(int *x, int y)
{
    *x += y;
}

void sub(int *x, int y)
{
    *x -= y;
}

void mul(int *x, int y)
{
    *x *= y;
}
