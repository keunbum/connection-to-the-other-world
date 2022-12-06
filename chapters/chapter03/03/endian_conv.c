#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    uint16_t host_port = 0x1234;
    uint16_t net_port;
    uint32_t host_addr = 0x12345678;
    uint32_t net_addr;

    net_port = htons(host_port);
    net_addr = htonl(host_addr);

    printf("Host ordered port: %#x\n", host_port);
    printf("Network ordered port: %#x\n", net_port);
    printf("Host ordered address: %#x\n", host_addr);
    printf("Network ordered address: %#x\n", net_addr);

    return 0;
}