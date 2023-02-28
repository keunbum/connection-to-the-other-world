#include <arpa/inet.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
#define my_bind(...) no_result_used_my_func(bind, __VA_ARGS__)
#define my_close(...) no_result_used_my_func(close, __VA_ARGS__)
#define my_recvfrom(recv_bytes, ...) my_func(recvfrom, recv_bytes, __VA_ARGS__)
#define my_sendto(sent_bytes, ...) no_result_used_my_func(sendto, sent_bytes, __VA_ARGS__)

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

static void error_handling(const char *);

int main(int argc, char *argv[])
{
#define BUF_SIZE (32)

    int serv_sock;
    static char msg_buf[BUF_SIZE];
    struct sockaddr_in serv_addr;

    // verify argc
    if (argc != 2)
    {
        printf("usage: %s <PORT>\n", argv[0]);
        exit(1);
    }

    // ready
    my_socket(serv_sock, PF_INET, SOCK_DGRAM, 0); // SOCK_DGRAM!!
    set_addr_int_ip(serv_addr, AF_INET, INADDR_ANY, argv[1]);
    my_bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // do it
    while (1)
    {
        static struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_size = sizeof(clnt_addr);
        ssize_t recv_msg_len;
        my_recvfrom(recv_msg_len, serv_sock, msg_buf, sizeof(msg_buf), 0, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (recv_msg_len == 0)
        {
            break;
        }
        my_sendto(serv_sock, msg_buf, recv_msg_len, 0, (struct sockaddr *)&clnt_addr, clnt_addr_size);
    }

    // clean up
    my_close(serv_sock);

    return 0;
}

static void error_handling(const char *message)
{
    perror(message);
    exit(1);
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