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
#define my_listen(...) no_result_used_my_func(listen, __VA_ARGS__)
#define my_accept(fd, ...) my_func(accept, fd, __VA_ARGS__)
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
#define MAX_QUEUE_SIZE (5)

static void error_handling(const char *);
static void process_exit_child(int);
static int main2(char *[]);

int main(int argc, char *argv[])
{
    // verify argc
    if (argc != 2)
    {
        printf("usage: %s <PORT>\n", argv[0]);
        exit(1);
    }
    return main2(argv);
}

static int main2(char *argv[])
{
    struct sigaction act = {};
    act.sa_handler = process_exit_child;
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    int serv_sock;
    struct sockaddr_in serv_addr;
    my_socket(serv_sock, PF_INET, SOCK_STREAM, 0);
    set_addr_int_ip(serv_addr, AF_INET, INADDR_ANY, argv[1]);
    my_bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    my_listen(serv_sock, MAX_QUEUE_SIZE);
    while (true)
    {
        struct sockaddr_in clnt_addr;
        socklen_t addr_size = (socklen_t)sizeof(clnt_addr);
        int clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &addr_size);
        if (clnt_sock == -1)
        {
            continue;
        }
        puts("new client connected...");
        pid_t pid = fork();
        if (pid == 0)
        {
            close(serv_sock);
            static char buf[BUF_SIZE];
            int str_len;
            while ((str_len = read(clnt_sock, buf, sizeof(buf))) != 0)
            {
                if (str_len == -1)
                {
                    break;
                }
                write(clnt_sock, buf, str_len);
            }
            close(clnt_sock);
            puts("client disconnected...");
            return 0;
        }
        close(clnt_sock);
    }
    my_close(serv_sock);

    return 0;
}

static void error_handling(const char *message)
{
    perror(message);
    exit(1);
}

static void process_exit_child(int sig)
{
    assert(sig == SIGCHLD);
    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);
    assert(pid != -1);
    printf("removed proc id: %d\n", pid);
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