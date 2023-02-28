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
#define my_connect(...) no_result_used_my_func(connect, __VA_ARGS__)
#define my_write(...) no_result_used_my_func(write, __VA_ARGS__)
#define my_read(read_bytes, ...) my_func(read, read_bytes, __VA_ARGS__)

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
static bool is_quit_cmd(const char *);

int main(int argc, char *argv[])
{
#define BUF_SIZE (1024)
    int my_sock;
    static char msg_buf[BUF_SIZE];
    struct sockaddr_in serv_addr;

    // verify argc
    if (argc != 3)
    {
        printf("usage: %s <IP> <PORT>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // ready
    my_socket(my_sock, PF_INET, SOCK_DGRAM, 0);
    set_addr_str_ip(serv_addr, AF_INET, argv[1], argv[2]);
    my_connect(my_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    // do it
    while (true)
    {
        fputs("Enter message to send(q to quit): ", stdout);
        fgets(msg_buf, sizeof(msg_buf), stdin);
        if (is_quit_cmd(msg_buf))
        {
            break;
        }
        my_write(my_sock, msg_buf, strlen(msg_buf));
        ssize_t msg_len;
        my_read(msg_len, my_sock, msg_buf, sizeof(msg_buf) - 1);
        msg_buf[msg_len] = '\0';
        printf("msg from server: %s", msg_buf);
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

static bool is_quit_cmd(const char *cmd)
{
    static char *TERMINATE_CMD[] = {"q\n", "Q\n"};
    bool is_quit = false;
    forn(i, sizeof(TERMINATE_CMD) / sizeof(TERMINATE_CMD[0]))
    {
        is_quit |= (strcmp(cmd, TERMINATE_CMD[i]) == 0);
    }
    return is_quit;
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