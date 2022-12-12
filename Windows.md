# Some Error Resolution Or Useful Tips in Windows.

## Index

* [Visual Studio](#visual-studio)
    * [template](#template)
* [CMD](#cmd)
    * [compile](#compile)
	* [background exec](#background-exec)
* [Programming](#programming)
    * [Data Types](#data-types)

## Visual Studio

### template

Use the following template code for network socket programming on Windows.

<ex_server_win.c>
```c
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define ForN(i, n) for (int i = 0; i < (int)(n); ++i)

#define Eprintf(...) fprintf(stderr, __VA_ARGS__)

#define MyFunc(func, res, ...)                               \
    do                                                       \
    {                                                        \
        if ((res = func(__VA_ARGS__)) == -1)                 \
        {                                                    \
            ErrorHandling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                    \
    } while (0)

#define MySocket(res, ...) MyFunc(socket, res, __VA_ARGS__)
#define MyBind(res, ...) MyFunc(bind, res, __VA_ARGS__)
#define MyListen(res, ...) MyFunc(listen, res, __VA_ARGS__)
#define MyAccept(res, ...) MyFunc(accept, res, __VA_ARGS__)
#define MyRecv(res, ...) MyFunc(recv, res, __VA_ARGS__)
#define MySend(res, ...) MyFunc(send, res, __VA_ARGS__)
#define MyClosesocket(res, ...) MyFunc(closesocket, res, __VA_ARGS__)

#define SetAddr(addr, domain, ip, port)    \
    do                                     \
    {                                      \
        memset(&addr, 0, sizeof(addr));	   \
        addr.sin_family = domain;          \
        addr.sin_addr.s_addr = htonl(ip);  \
        addr.sin_port = htons(atoi(port)); \
    } while (0);

#define BUF_SIZE (128)
#define REQUEST_QUEUE_SIZE 5
#define CLIENT_ITER_NUM 5

void ErrorHandling(const char*);

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	int res;
	SOCKET servSock;
	SOCKADDR_IN servAddr;
	SOCKADDR_IN clntAddr;
	int clntAddrLen;

	static char msgBuf[BUF_SIZE];

	if (argc != 2)
	{
		printf("usage: %s <port>\n", argv[0]);
		exit(1);
	}

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() error");
	}

	MySocket(servSock, PF_INET, SOCK_STREAM, 0);

	SetAddr(servAddr, AF_INET, INADDR_ANY, argv[1]);
	MyBind(res, servSock, (struct sockaddr*)&servAddr, sizeof(servAddr));

	MyListen(res, servSock, REQUEST_QUEUE_SIZE);

	clntAddrLen = sizeof(msgBuf);
	ForN(i, CLIENT_ITER_NUM)
	{
		int readBytes;
		int clntSock;

		MyAccept(clntSock, servSock, (struct sockaddr*)&clntAddr, &clntAddrLen);

		Eprintf("%dth client...\n", i + 1);

		while ((readBytes = recv(clntSock, msgBuf, BUF_SIZE - 1, 0)) > 0)
		{
			MySend(res, clntSock, msgBuf, readBytes, 0);
		}
		if (readBytes == -1)
		{
			ErrorHandling("recv() error");
		}
		MyClosesocket(res, clntSock);
	}

	MyClosesocket(res, servSock);

	WSACleanup();
	return 0;
}

void ErrorHandling(const char* message)
{
	perror(message);
	exit(1);
}
```

<ex_client_win.c>
```c
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define ForN(i, n) for (int i = 0; i < (int)(n); ++i)

#define Eprintf(...) fprintf(stderr, __VA_ARGS__)

#define MyFunc(func, res, ...)                               \
    do                                                       \
    {                                                        \
        if ((res = func(__VA_ARGS__)) == -1)                 \
        {                                                    \
            ErrorHandling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                    \
    } while (0)

#define MySocket(res, ...) MyFunc(socket, res, __VA_ARGS__)
#define MyConnect(res, ...) MyFunc(connect, res, __VA_ARGS__)
#define MyRecv(res, ...) MyFunc(recv, res, __VA_ARGS__)
#define MySend(res, ...) MyFunc(send, res, __VA_ARGS__)
#define MyClosesocket(res, ...) MyFunc(closesocket, res, __VA_ARGS__)

#define SetAddr(addr, domain, ip, port)       \
    do                                        \
    {                                         \
        memset(&addr, 0, sizeof(addr));       \
        addr.sin_family = domain;             \
        addr.sin_addr.s_addr = inet_addr(ip); \
        addr.sin_port = htons(atoi(port));    \
    } while (0);

#define BUF_SIZE (64)
#define TERMINATE_MSG_1 "Q\n"
#define TERMINATE_MSG_2 "q\n"

void ErrorHandling(const char*);

int main(int argc, char* argv[])
{
    WSADATA wsaData;
    int res;
    int sock;
    struct sockaddr_in servAddr;
    static char msgBuf[BUF_SIZE];

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        ErrorHandling("WSAStartup() error");
    }

    MySocket(sock, PF_INET, SOCK_STREAM, 0);
    SetAddr(servAddr, AF_INET, argv[1], argv[2]);
    MyConnect(res, sock, (struct sockaddr*)&servAddr, sizeof(servAddr));

    while (1)
    {
        int strLen;
        int readBytes;
        int totalReadBytes;

        fputs("Please enter the message you want to send(Q to quit): ", stdout);
        fgets(msgBuf, BUF_SIZE, stdin);
        if (strcmp(msgBuf, TERMINATE_MSG_1) == 0 || strcmp(msgBuf, TERMINATE_MSG_2) == 0)
        {
            break;
        }

        strLen = strlen(msgBuf);
        MySend(res, sock, msgBuf, strLen, 0);

        totalReadBytes = 0;
        while (totalReadBytes < strLen)
        {
            MyRecv(readBytes, sock, msgBuf + totalReadBytes, BUF_SIZE - 1, 0);
            totalReadBytes += readBytes;
        }
        msgBuf[totalReadBytes] = '\0';
        printf("Message from server: %s", msgBuf);
    }

    MyClosesocket(res, sock);

    WSACleanup();
    return 0;
}

void ErrorHandling(const char* message)
{
    perror(message);
    exit(1);
}

```

## CMD

### compile

for more [info](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax?view=msvc-170)

```cmd
> cl main.c
```

### background exec

```cmd
$ start .\main.exe
```

## Programming

### Data Types

리눅스에서 ```uint16_t```나 ```uint32_t```를 쓰는 것처럼  
윈도우에서는 ```u_short```와 ```u_int``` 사용.

