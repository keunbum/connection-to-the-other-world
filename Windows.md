# Some Error Resolution Or Useful Tips in Windows.

## Visual Studio

### programming

when network socket programming

```c
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[])
{
    // ...

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
		ErrorHandling("WSAStartup() error!");
	}

    // ...

	WSACleanup();
	return 0;
}

...
```

### complie

for more [info](https://learn.microsoft.com/en-us/cpp/build/reference/compiler-command-line-syntax?view=msvc-170)

```cmd
> cl main.c
```