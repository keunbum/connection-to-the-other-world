#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

#define FORN(i, n) for (int i = 0; i < (int)(n); ++i)

#ifdef LOCAL
#define EPrintf(...) fprintf(stderr, __VA_ARGS__)
#define MyDebugStr(pref, str)                       \
    do                                              \
    {                                               \
        EPrintf(#pref ": " #str ": \"%s\"\n", str); \
    } while (0)
#define MyDebugArr(pref, arr, n, ch)          \
    do                                        \
    {                                         \
        EPrintf(#pref ": " #arr "[" #n "]:"); \
        FORN(i, n)                            \
        {                                     \
            EPrintf(" " #ch, arr[i]);         \
        }                                     \
        EPrintf("\n");                        \
    } while (0)
#else
#define EPrintf(...) (void)0
#define MyDebugStr(...) (void)0
#define MyDebugArr(...) (void)0
#endif

#define MyFunc(func, res, fail_value, ...)                    \
    do                                                        \
    {                                                         \
        EPrintf(#func "(" #__VA_ARGS__ ")\n");                \
        if ((res = func(__VA_ARGS__)) == fail_value)          \
        {                                                     \
            ErrorHandling(#func "(" #__VA_ARGS__ ") error");  \
        }                                                     \
    } while (0)
#define NoResultUsedMyFunc(func, fail_value, ...)   \
    do                                              \
    {                                               \
        int res;                                    \
        MyFunc(func, res, fail_value, __VA_ARGS__); \
    } while (0)

#define MySocket(sock, ...) MyFunc(socket, sock, INVALID_SOCKET, __VA_ARGS__)
#define MyCloseSocket(...) NoResultUsedMyFunc(closesocket, SOCKET_ERROR, __VA_ARGS__)
#define MyBind(...) NoResultUsedMyFunc(bind, SOCKET_ERROR, __VA_ARGS__)
#define MyConnect(...) NoResultUsedMyFunc(connect, SOCKET_ERROR, __VA_ARGS__)
#define MyRecvFrom(recv_bytes, ...) MyFunc(recvfrom, recv_bytes, SOCKET_ERROR, __VA_ARGS__)
#define MySendTo(...) NoResultUsedMyFunc(sendto, SOCKET_ERROR, __VA_ARGS__)
#define MySend(...) NoResultUsedMyFunc(send, SOCKET_ERROR, __VA_ARGS__)
#define MyRecv(read_bytes, ...) MyFunc(recv, read_bytes, SOCKET_ERROR, __VA_ARGS__)


// addr's type: SOCKADDR_IN
// domain's type: int
// ip's type: u_long
// port's type: char *
#define SetAddrIntIP(addr, domain, ip, port)    \
    do                                          \
    {                                           \
        memset(&(addr), 0, sizeof(addr));       \
        addr.sin_family = domain;               \
        addr.sin_addr.s_addr = htonl(ip);       \
        addr.sin_port = htons(atoi(port));      \
    } while (0)

// addr's type: SOCKADDR_IN
// domain's type: int
// ip's type: const char *
// port's type: char *
#define SetAddrStrIP(addr, domain, ip, port)    \
    do                                          \
    {                                           \
        memset(&(addr), 0, sizeof(addr));       \
        addr.sin_family = domain;               \
        addr.sin_addr.s_addr = inet_addr(ip);   \
        addr.sin_port = htons(atoi(port));      \
    } while (0)


#define BUF_SIZE (@@here@@)

static void ErrorHandling(const char*);

void Main(char* argv[])
{
	SOCKET @@here@@;
	SOCKADDR_IN @@here@@;

	// clean up
	MyCloseSocket(@@here@@);
}

int main(int argc, char* argv[])
{
	// verify argc
	if (argc != 2)
	{
		printf("usage: %s <PORT>\n", argv[0]);
		exit(1);
	}

	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		ErrorHandling("WSAStartup() Error");
	}

	Main(argv);

	WSACleanup();

	return 0;
}

static void ErrorHandling(const char* message)
{
	perror(message);
	exit(1);
}

/* read it as if you were wrong once. --> "why is this wrong??"

 * basic strategy:
  * don't be obsessed with speed or memory when the input is small compared to limit
  * internalization of problem statements
  * simplify. a step-by-step approach
  * readability is important
  * think rationally. keep calm
  * DON'T GET STUCK ON ONE APPROACH

 * stuff you should look for
  * null pointer exception
  * invalid argument passing
  * invalid error handling
  * special cases (zero size msg?)
*/