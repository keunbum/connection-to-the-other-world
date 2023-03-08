#include <arpa/inet.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)

#ifdef LOCAL
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define my_debug_str(pref, str)                     \
    do                                              \
    {                                               \
        eprintf(#pref ": " #str ": \"%s\"\n", str); \
    } while (0)
#define my_debug_arr(pref, arr, n, ch)        \
    do                                        \
    {                                         \
        eprintf(#pref ": " #arr "[" #n "]:"); \
        forn(i, n)                            \
        {                                     \
            eprintf(" " #ch, arr[i]);         \
        }                                     \
        eprintf("\n");                        \
    } while (0)
#else
#define eprintf(...) (void)0
#define my_debug_str(...) (void)0
#define my_debug_arr(...) (void)0
#endif

#define my_func(func, res, ...)                               \
    do                                                        \
    {                                                         \
        eprintf(#func "(" #__VA_ARGS__ ")\n");                \
        if ((res = func(__VA_ARGS__)) == -1)                  \
        {                                                     \
            error_handling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                     \
    } while (0)
#define no_result_used_my_func(func, ...) \
    do                                    \
    {                                     \
        int res;                          \
        my_func(func, res, __VA_ARGS__);  \
    } while (0)

#define my_socket(sock, ...) my_func(socket, sock, __VA_ARGS__)
#define my_close(...) no_result_used_my_func(close, __VA_ARGS__)
#define my_bind(...) no_result_used_my_func(bind, __VA_ARGS__)
#define my_connect(...) no_result_used_my_func(connect, __VA_ARGS__)
#define my_recvfrom(recv_bytes, ...) my_func(recvfrom, recv_bytes, __VA_ARGS__)
#define my_sendto(...) no_result_used_my_func(sendto, __VA_ARGS__)
#define my_write(...) no_result_used_my_func(write, __VA_ARGS__)
#define my_read(read_bytes, ...) my_func(read, read_bytes, __VA_ARGS__)

// addr's type: struct sockaddr_in
// domain's type: int
// ip's type: uint32_t
// port's type: char *
#define set_addr_int_ip(addr, domain, ip, port) \
    do                                          \
    {                                           \
        memset(&(addr), 0, sizeof(addr));       \
        addr.sin_family = domain;               \
        addr.sin_addr.s_addr = htonl(ip);       \
        addr.sin_port = htons(atoi(port));      \
    } while (0)

// addr's type: struct sockaddr_in
// domain's type: int
// ip's type: const char *
// port's type: char *
#define set_addr_str_ip(addr, domain, ip, port) \
    do                                          \
    {                                           \
        memset(&(addr), 0, sizeof(addr));       \
        addr.sin_family = domain;               \
        addr.sin_addr.s_addr = inet_addr(ip);   \
        addr.sin_port = htons(atoi(port));      \
    } while (0)

#define BUF_SIZE (128)

static void error_handling(const char *);
static void write_routine(int, char *);
static void read_routine(int, char *);
static int main2(char *[]);

int main(int argc, char *argv[])
{

    // verify argc
    if (argc != 3)
    {
        printf("usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    return main2(argv);
}

static int main2(char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    my_socket(sock, PF_INET, SOCK_STREAM, 0);
    set_addr_str_ip(serv_addr, AF_INET, argv[1], argv[2]);
    my_connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    pid_t pid = fork();
    static char buf[BUF_SIZE];
    if (pid == 0)
    {
        write_routine(sock, buf);
    }
    else
    {
        read_routine(sock, buf);
    }

    close(sock);
    return 0;
}

static void error_handling(const char *message)
{
    perror(message);
    exit(1);
}

static void write_routine(int sock, char *buf)
{
    while (true)
    {
        fgets(buf, BUF_SIZE, stdin);
        if (strcmp(buf, "q\n") == 0 || strcmp(buf, "Q\n") == 0)
        {
            shutdown(sock, SHUT_WR);
            return;
        }
        write(sock, buf, strlen(buf));
    }
}

static void read_routine(int sock, char *buf)
{
    int read_bytes;
    while ((read_bytes = read(sock, buf, BUF_SIZE)) != 0)
    {
        buf[read_bytes] = '\0';
        printf("Message from server: %s", buf);
    }
    if (read_bytes == -1)
    {
        error_handling("read() error");
    }
}

/* read it as if you were wrong once. --> "why is this wrong??"

 * basic strategy:
  * don't be obsessed with speed or memory when the input is small compared to limit
  * internalization of problem statements
  * simplify. a step-by-step approach
  * readability is important
  * think rationally. keep calm
  * DON'T GET STUCK ON ONE APPROACH

 * stuff you should look for
  * null pointer exception
  * invalid argument passing
  * invalid error handling
  * special cases (zero size msg?)
*/