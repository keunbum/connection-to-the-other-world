#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid = fork();

    if (pid == 0)
    {
        puts("I'm Child.");
    }
    else
    {
        puts("I'm Parent");
        sleep(10);
    }

    if (pid == 0)
    {
        puts("End Child");
    }
    else
    {
        puts("End Parent");
    }

    return 0;
}