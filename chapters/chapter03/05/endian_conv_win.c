#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(const char*);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	u_short host_port = 0x1234;
	u_short net_port;
	u_long host_addr = 0x12345678;
	u_long net_addr;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	net_port = htons(host_port);
	net_addr = htonl(host_addr);

	printf("Host ordered port: %#x\n", host_port);
	printf("Network ordered port: %#x\n", net_port);
	printf("Host ordered addr: %#x\n", host_addr);
	printf("Network ordered addr: %#x\n", net_addr);

	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	perror(message);
	exit(1);
}