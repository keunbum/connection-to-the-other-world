#define _WINSOCK_DEPRECATED_NO_WARNINGS

#undef UNICODE
#undef _UNICODE

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

	{
		char* strAddr = "203.211.218.102:9190";
		char strAddrBuf[50];
		SOCKADDR_IN servAddr;
		int addrLength;

		addrLength = sizeof(servAddr);
		WSAStringToAddress(
			strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &addrLength);

		addrLength = sizeof(strAddrBuf);
		WSAAddressToString(
			(SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &addrLength);

		printf("Second conv result: %s\n", strAddrBuf);
	}

	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	perror(message);
	exit(1);
}