# Some Error Resolution Or Useful Tips in Programming, Git, Etc.

## Index

* [VS Code](#vs-code)
    * [단축키](#단축키)
* [GCC](#gcc)
    * [errno 출력](#errno-출력)
* [Bash](#bash)
    * [compile](#compile)
    * [background exec](#background-exec)
* [Git](#git)
    * [git push](#git-push)

## VS Code

### 단축키

* #### 자동 정렬
    1. Windows: Shift + Alt + F

* #### MarkDown Preview
    1. Windows: Ctrl + Shift + V

* #### Terminal Switching
    1. Windows: Ctrl + `

## GCC

### errno 출력

편의상 close() 함수를 이용하여 에러 유도.

```c
/* main.c */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

void my_close_perror(int);
void my_close_strerror(int);

int main(void)
{
    int fd = -1;
    my_close_perror(fd);
    my_close_strerror(fd);
    return 0;
}

void my_close_perror(int fd)
{
    if (close(fd) == -1)
    {
    	perror("my_close(): error");
    }
}

void my_close_strerror(int fd)
{
    if (close(fd) == -1)
    {
    	fputs(strerror(errno), stderr);
    	fputc('\n', stderr);
    }
}
```

```txt
# 실행 결과
$ ./main
my_close(): error: Bad file descriptor
Bad file descriptor
```

## Bash

### compile

In **~/.bashrc** just put like this.

```bash
# some my functions
c() {
    # compile $1.c to $1
    gcc "$1".c -o "$1"

    # If useful..
    # ./"$1"
    # rm "$1"
}
```

and then,
```
$ source ~/.bashrc
```

By this, you don't need to put like this,
```bash
$ gcc my_foo_bar_baz.c -o my_foo_bar_baz
```

just
```bash
$ c my_foo_bar_baz
```

### background exec

```bash
$ ./main &
```

## Git

### git push

* [git wsl documentation bug git credential manager core.exe not found](https://github.com/microsoft/WSL/issues/8395#issuecomment-1121629434)
