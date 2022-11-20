# Some Error Resolution Or Useful Tips in Windows.

## Visual Studio

### programming

* when network socket programming

```c
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <WinSock2.h>

#pragma comment(lib, "ws2_32.lib")

...
```

### complie

```cmd
> cl main.c
```