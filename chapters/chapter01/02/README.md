# Index
   * ## [리눅스 기반 파일 조작하기](#리눅스-기반-파일-조작하기-1)
      * ### [저수준 파일 입출력(Low-level File Access)](#저수준-파일-입출력low-level-file-access-1)
      * ### [파일 열기](#파일-열기-1)
      * ### [파일 닫기](#파일-닫기-1)
      * ### [파일에 데이터 쓰기](#파일에-데이터-쓰기-1)
      * ### [파일에 저장된 데이터 읽기](#파일에-저장된-데이터-읽기-1)
      * ### [파일 디스크립터와 소켓](#파일-디스크립터와-소켓-1)

   * ## [How to Execute](#how-to-execute-1)
      * ### [Reference](#reference-1)
      
# 리눅스 기반 파일 조작하기

## 저수준 파일 입출력(Low-level File Access)

여기서 말하는 저수준이란 "표준에 상관없이 운영체제가 독립적으로 제공하는~"의 의미로 받아들이면 된다.
즉, 이후에 설명하는 함수들은 리눅스에서 제공하는 함수이지, ANSI 표준에서 정의한 함수가 아니라는 뜻이다.

## 파일 열기

[open()](https://man7.org/linux/man-pages/man2/open.2.html)

데이터를 읽거나 쓰기 위해 파일을 열 때 사용하는 함수이다.

open() 함수 호출은 새로운 **open file description**(열려 있는 파일 테이블에 대한 엔트리)을 만들고
**open file description**은 **file offset**과 **file status flags**를 기록한다.
**file descriptor**란 **open file description**에 대한 참조라고 볼 수 있다.

이때 **flags**는 크게 세 가지로 나뉜다. 이때 **file access mode**는 반드시 지정해야 한다.
  * **file access mode**
    * O_RDONLY, O_WRONLY, O_RDWR
  * **file creation flags**
    * O_CLOEXEC, O_CREAT, O_DIRECTORY, O_EXCL, O_NOCTTY, O_NOFOLLOW, O_TMPFILE, and O_TRUNC.
  * **file status flags**
    * O_APPEND, O_NONBLOCK, O_ASYNC, ...
  
**file creation flags**는 열기 행위 자체에 대한 특징을 지정하고, **file status flags**는 연이은 I/O 행위의 특징을 지정한다.


```c
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int open(const char *path, ing flags);
// -> 성공 시 파일 디스크립터 반환, 실패 시 -1 반환
// open(const char *path, ing flags, mode_t mode) 꼴의 함수도 제공하지만 잘 쓰이지는 않을 것.
```

| flag     | feature                     | category            |
|----------|-----------------------------|---------------------|
| O_CREAT  | create file if needed       | file creation flags |
| O_TRUNC  | delete all existing data    | file creation flags |
| O_APPEND | continue from existing data | file status flags   |
| O_RDONLY | read only                   | file access mode    |
| O_WRONLY | write only                  | file access mode    |
| O_RDWR   | read/write both             | file access mode    |

## 파일 닫기

[close()](https://man7.org/linux/man-pages/man2/close.2.html)

파일은 사용 후 반드시 닫아줘야 한다.

함수를 호출하면서 파일 디스크립터를 인자로 전달하면 해당 파일은 닫히게(종료하게) 된다.
소켓을 닫을 때에도 사용됨에 유의하자.

close() 함수 호출이 에러가 났다고 다시 close()를 시도해서는 안된다고 한다.
(여러 프로세스가 동일한 파일 디스크립터를 참조하고 있는 경우 close()는 신중하게 해야 하는 듯. 자세한 건 man 페이지를 읽어 보도록.)


```c
#include <unistd.h>

int close(int fd);
// -> 성공 0, 실패 시 -1 반환
```

## 파일에 데이터 쓰기

## 파일에 저장된 데이터 읽기

## 파일 디스크립터와 소켓

---

# How To Execute

## low_open.c

```bash
$ gcc low_open.c -o low_open
$ ./low_open
$ cat data.txt
```

## low_read.c
```bash
$ gcc low_read.c -o low_read
$ ./low_read
```

## fd_seri.c
```bash
$ gcc fd_seri.c -o fd_seri
$ ./fd_seri
```

## Reference
* [man 2 write](https://man7.org/linux/man-pages/man2/write.2.html)
* [man 2 read](https://man7.org/linux/man-pages/man2/read.2.html)
