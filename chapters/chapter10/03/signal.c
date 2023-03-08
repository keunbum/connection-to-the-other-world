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

void keycontrol(int sig)
{
    assert(sig == SIGINT);
    puts("CTRL+C pressed");
}

int main(int argc, char *argv[])
{
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);
    for (int _ = 0; _ < 3; ++_)
    {
        puts("wait...");
        sleep(100);
    }
    return 0;
}