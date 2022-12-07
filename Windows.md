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
```c : template.c
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

���������� ```uint16_t```�� ```uint32_t```�� ���� ��ó��  
�����쿡���� ```u_short```�� ```u_int``` ���.

