#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define ForN(i, n) for (int i = 0; i < (int)(n); ++i)

#define Eprintf(...) fprintf(stderr, __VA_ARGS__)

#define MyFunc(func, res, ...)                               \
    do                                                        \
    {                                                         \
        if ((res = func(__VA_ARGS__)) == -1)                  \
        {                                                     \
            ErrorHandling(#func "(" #__VA_ARGS__ ") error"); \
        }                                                     \
    } while (0)

#define MySocket(res, ...) MyFunc(socket, res, __VA_ARGS__)
#define MyConnect(res, ...) MyFunc(connect, res, __VA_ARGS__)
#define MyRecv(res, ...) MyFunc(recv, res, __VA_ARGS__)
#define MySend(res, ...) MyFunc(send, res, __VA_ARGS__)
#define MyClosesocket(res, ...) MyFunc(closesocket, res, __VA_ARGS__)

#define SetAddr(addr, domain, ip, port)           \
    do                                             \
    {                                              \
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
