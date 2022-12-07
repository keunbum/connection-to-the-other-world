# 03-4 인터넷 주소의 초기화와 할당

---

## 문자열 정보를 네트워크 바이트 순서의 정수로 변환하기

<details>
<summary>inet_addr</summary>
<div markdown="1">

```c
#include <arpa/inet.h>

in_addr_t inet_addr(const char *string);
// -> 성공 시 빅 엔디안으로 변환된 32비트 정수 값, 실패 시 INADDR_NONE 반환
```

1) 문자열("127.0.0.1" 형식) -> 32비트 정수형
2) 네트워크 바이트 순서(빅 엔디안)로 변환
3) 오류(잘못된 형식의 IP 주소) 검출

```c : inet_addr.c
#include <stdio.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    const char *addrs[] = {"1.2.3.4", "1.2.3.256"};

    for (int i = 0; i < 2; ++i)
    {
        uint32_t conv_addr = inet_addr(addrs[i]);
        if (conv_addr == INADDR_NONE)
        {
            puts("Error occured!");
        }
        else
        {
            printf("Network ordered integer addr: %#x\n", conv_addr);
        }
    }
    return 0;
}
```

사실 얘보다는 가급적 밑에서 소개하는 친구를 쓰자.
</div>
</details>

<details>
<summary>inet_aton</summary>
<div markdown="1">

```c
#include <arpa/inet.h>

int inet_aton(const char *string, struct in_addr *addr);
// -> 성공 시 1(true), 실패 시 0(false) 반환
```

1) 별도의 대입 과정 필요X

```c : inet_aton.c
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>

void error_handling(const char *);

int main(int argc, char *argv[])
{
    char *addr = "127.232.124.79";
    struct sockaddr_in addr_inet;

    if (!inet_aton(addr, (struct in_addr *)&addr_inet.sin_addr))
    {
        error_handling("inet_aton error");
    }
    printf("Network ordered integer addr: %#x\n", addr_inet.sin_addr.s_addr);
    return 0;
}

void error_handling(const char *message)
{
    perror(message);
    exit(1);
}
```
</div>
</details>


<details>
<summary>inet_ntoa</summary>
<div markdown="1">

```c
#include <arpa/inet.h>

char *inet_ntoa(struct in_addr adr);
// -> 성공 시 변환된 문자열의 주소 값, 실패 시 -1 반환 (근데 사실 상 실패할 리가...)
```

1) 정수 형태 IP정보 -> 문자열 형식 IP로 변환
2) 함수 내부적으로 공간할당하여 주솟값 반환 -> 다시 호출할 경우 값이 덮어써지므로 값을 다른 메모리 공간에 복사해 둘 것.

```c : inet_ntoa.c
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    struct sockaddr_in addr1, addr2;
    char *str_ptr = NULL;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("Dotted-Decimal notation1: %s\n", str_ptr);

    inet_ntoa(addr2.sin_addr);
    printf("Dotted-Decimal notation2: %s\n", str_ptr);
    printf("Dotted-Decimal notation3: %s\n", str_arr);

    return 0;
}
```

[실행 결과]
```bash
$ ./inet_ntoa
Dotted-Decimal notation1: 1.2.3.4
Dotted-Decimal notation2: 1.1.1.1
Dotted-Decimal notation3: 1.2.3.4
```
</div>
</details>


---

## 인터넷 주소의 초기화

소켓 생성 과정에서 흔히 등장하는 인터넷 주소정보 초기화 코드 예시.
```c
struct sockaddr_in addr;
char *serv_ip = "211.217.168.13";
char *serv_port = "9190";
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = inet_addr(serv_ip);
addr.sin_port = htons(atoi(port));
```

보통은 프로그램 실행 시 main 함수에 IP와 PORT번호를 전달하도록 구성.

---

## 클라이언트의 주소정보 초기화

별 내용 X

---

## INADDR_ANY

서버 소켓의 생성과정에서 매번 서버의 IP주소를 입력하는 것은 귀찮을 수 있다. 그래서 다음과 같이 주소 정보를 초기화하곤 한다.

```c
struct sockaddr_in addr;
char *serv_ip = "211.217.168.13";
char *serv_port = "9190";
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_addr.s_addr = htonl(INADDR_ANY);
addr.sin_port = htons(atoi(port));
```

예를 들어 컴퓨터에 2개의 랜카드가 설치되어 있다면, 할당 받은 IP중 어떤 주소를 통해서 데이터가 들어오더라도 PORT번호만 일치하면 수신할 수 있게 된다. 특히 NIC가 하나뿐인 컴퓨터라면 주저 없이 INADDR_ANY를 이용해서 초기화하는 것이 편리하다.


---

## Chapter01의 hello_server.c, hello_client.c의 실행에 대한 고찰


별 내용 X


---

## 소켓에 인터넷 주소 할당하기

구조체 sockaddr_in을 초기화 했으니, 초기화된 주소정보를 소켓에 할당하는 일만 남았다. 다음에 소개하는 bind 함수가 바로 이러한 역할을 담당한다.

```c
#include <sys/socket.h>

int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
// -> 성공 시 0, 실패 시 -1 반환
```

| variable | role                                                    |
|----------|---------------------------------------------------------|
| sockfd   | 주소정보(IP, PORT)를 할당할 소켓의 파일 디스크립터      |
| addr     | 할당하고자 하는 주소정보를 지니는 구조체 변수의 주소 값 |
| addrlen  | 두 번째 인자로 전달된 구조체 변수의 길이 정보           |

