#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

void timeout(int sig)
{
    assert(sig == SIGALRM);
    puts("Time out!");
    alarm(2);
}

#define SET_SIGACTION(act, func) \
    do                           \
    {                            \
        act.sa_handler = func;   \
        act.sa_flags = 0;        \
    } while (0)

int main(int argc, char *argv[])
{
    struct sigaction act = {};
    SET_SIGACTION(act, timeout);
    sigaction(SIGALRM, &act, NULL);
    alarm(2);
    for (int _ = 0; _ < 3; ++_)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}