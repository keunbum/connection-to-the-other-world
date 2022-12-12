#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

void ErrorHandling(const char*);

int main(int argc, char* argv[])
{
	WSADATA wsaData;

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) 
	{
		ErrorHandling("WSAStartup() error");
	}


	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	perror(message);
	exit(1);
}