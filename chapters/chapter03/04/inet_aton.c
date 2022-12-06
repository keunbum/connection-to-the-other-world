#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(const char *);

int main(int argc, char *argv[])
{
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if (!inet_aton(addr, (struct in_addr *)&addr_inet.sin_addr))
    {
        error_handling("inet_aton error");
    }
    printf("Network ordered integer addr: %#x\n", addr_inet.sin_addr.s_addr);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}
