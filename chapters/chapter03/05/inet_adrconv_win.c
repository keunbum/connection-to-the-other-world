//#define _WINSOCK_DEPRECATED_NO_WARNINGS

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

	/* inet_addr 함수 호출 예 */
	{
		const char* addr = "127.212.124.78";
		u_long conv_addr = inet_addr(addr);
		if (conv_addr == INADDR_NONE)
		{
			puts("Error occured!");
		}
		else
		{
			printf("Network ordered interger adrr: %#lx\n", conv_addr);
		}
	}

	/* inet_ntoa 함수 호출 예 */
	{
		struct sockaddr_in addr;
		char* strPtr = NULL;
		char strArr[20];

		addr.sin_addr.s_addr = htonl(0x1020304);
		strPtr = inet_ntoa(addr.sin_addr);
		strcpy(strArr, strPtr);
		printf("Dotted-Decimal notation3: %s\n", strArr);
	}

	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	perror(message);
	exit(1);
}