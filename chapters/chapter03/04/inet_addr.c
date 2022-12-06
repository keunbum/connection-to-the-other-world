#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    const char *addrs[] = {"1.2.3.4", "1.2.3.256"};

    for (int i = 0; i < 2; ++i)
    {
        uint32_t conv_addr = inet_addr(addrs[i]);
        if (conv_addr == INADDR_NONE)
        {
            puts("Error occured!");
        }
        else
        {
            printf("Network ordered integer addr: %#x\n", conv_addr);
        }
    }
    return 0;
}