#include <arpa/inet.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define forn(i, n) for (int i = 0; i < (int)(n); ++i)

#ifdef LOCAL
#define eprintf(...) fprintf(stderr, __VA_ARGS__)
#define my_debug_str(pref, str)                \
    do                                         \
    {                                          \
        eprintf(#pref #str ": \"%s\"\n", str); \
    } while (0)
#define my_debug_arr(pref, arr, n, ch)   \
    do                                   \
    {                                    \
        eprintf(#pref #arr "[" #n "]:"); \
        forn(i, n)                       \
        {                                \
            eprintf(" " #ch, arr[i]);    \
        }                                \
        eprintf("\n");                   \
    } while (0)
#else
#define eprintf(...) (void)0
#define my_debug_str(...) (void)0
#define my_debug_arr(...) (void)0
#endif

#define my_func(func, res, ...)                               \
    do                                                        \
    {                                                         \
        eprintf(#func "(" #__VA_ARGS__ ");\n");               \
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

static void error_handling(const char *);

int main(int argc, char *argv[])
{
#define BUF_SIZE (1024)
    int my_sock;
    struct sockaddr_in his_addr;

    // verify argc
    if (argc != 3)
    {
        printf("usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // ready
    my_socket(my_sock, PF_INET, SOCK_DGRAM, 0);
    set_addr_str_ip(his_addr, AF_INET, argv[1], argv[2]);

    // do it
#define ITER_NUM 3
    forn(i, ITER_NUM)
    {
        static char *MSGS[] = {
            "HI!",
            "I'm another UDP host!",
            "Nice to meet you", // trailing commas
        };
        my_sendto(my_sock, MSGS[i], strlen(MSGS[i]), 0, (struct sockaddr *)&his_addr, sizeof(his_addr));
    }

    // clean up
    my_close(my_sock);

    return EXIT_SUCCESS;
}

static void error_handling(const char *message)
{
    perror(message);
    exit(EXIT_FAILURE);
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