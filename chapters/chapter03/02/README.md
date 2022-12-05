# 03-2 주소 정보의 표현

앞선 예제에서 보았듯이 응용프로그램상에서의 IP주소와 PORT번호 표현을 위한 구조체가 정의되어 있다. 이 구조체를 중심으로 목적지 주소의 표현방법에 대해 살펴 보고조 한다. IPv4 중심으로 살펴봄에 유의한다.

---

## IPv4 기반의 주소표현을 위한 구조체

주소정보를 담을 때에는 다음 세 가지 물음에 답이 되도록 담아야 한다.

    - Q1) 어떠한 주소체계를 사용하나요?
    - A1) IPv4 기반 주소체계를 사용합니다.  

    - Q2) IP주소가 어떻게 되나요?
    - A2) 211.204.215.85입니다.  

    - Q3) PORT번호는 어떻게 되나요?
    - A3) 4954번입니다.

그리하여 다음의 형태로 구조체가 정의되었다.

```c
// 참고: https://man7.org/linux/man-pages/man7/ip.7.html
// 헤더 파일 Go to Definition 활용하기
#include <netinet/in.h>

struct sockaddr_in {
    sa_family_t    sin_family;   // 주소체계(Address Family). e.g. AF_INET
    in_port_t      sin_port;     // 16비트 TCP/UDP PORT번호. e.g. htons(3490)
    struct in_addr sin_addr;     // 32비트 IP주소. see struct in_addr, below
    char           sin_zero[8];  // 사용되지 않음. zero this if you want to
};

struct in_addr {
    in_addr_t s_addr;  // load with inet_aton()
};
```

uint16_t, in_addr_t와 같은 자료형의 근거는 POSIX(Portable Operating System Interface)에서 찾을 수 있다. POSIX란 유닉스 계열의 운영체제에 적용하기 위한 표준을 의미한다.

---

## 구조체 sockaddr_in의 멤버 분석

### 멤버 sin_family

프로토콜 체계마다 적용하는 주소체계가 다르다. 예를 들어서 IPv4에서는 4바이트 주소체계를 사용하고 IPv6에서는 16바이트 주소체계를 사용한다. 따라서 아래의 표를 참조하여 적용할 주소체계 정보를 멤버 sin_family에 저장해야 한다.

| Address Family |                     의미                    |
|:--------------:|:-------------------------------------------:|
|     AF_INET    | IPv4 인터넷 프로토콜에 적용하는 주소체계     |
|     AF_INET6   | IPv4 인터넷 프로토콜에 적용하는 주소체계     |
|     AF_LOCAL   | 로컬 통신을 위한 유닉스 프로토콜의 주소체계 |

### 멤버 sin_port

16비트 PORT번호를 저장한다. 단, **'네트워크 바이트 순서'로 저장**해야 한다.

### 멤버 sin_addr

32비트 IP주소정보를 저장한다. 이 역시 **'네트워크 바이트 순서'로 저장**해야 한다.
### 멤버 sin_zero

단순히 구조체 sockaddr_in의 크기를 구조체 sockaddr과 일치시키기 위해 삽입된 멤버이다. 그러나 반드시 0으로 채워야 한다. 이전 코드에서 보듯, sockaddr_in 구조체 변수의 주소 값은 bind 함수의 인자로 다음과 같이 전달된다.

```c
    struct sockaddr_in serv_addr;
    . . .
    if (bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
    {
        . . .
    } 
```

bind 함수는 주소체계, PORT번호, IP주소정보를 담고 있는 sockaddr 구조체 변수의 주소 값을 요구한다. 그런데 구조체 sockaddr은 다음과 같이 정의되어 있다.

```c
/* Structure describing a generic socket address.  */
struct sockaddr
  {
    sa_family_t sa_family;	/* Common data: address family and length.  */
    char sa_data[14];		/* Address data.  */
  };
```

구조체 멤버 sa_data에 저장되는 주소정보에는 IP주소와 PORT번호가 포함되어야 하고, 이 두 가지 정보를 담고 남은 부분은 0으로 채울 것을 bind 함수는 요구하고 있다. 그런데 이런 식의 선언은 주소정보를 담기에 불편하다. 그래서 구조체 sockaddr_in이 등장했다.

>🔖 **sin_family**  
> ... 대충 struct sockaddr과 맞추기 위해 struct sockaddr_in에도 멤버 변수 sin_family가 필요하다는 내용  
> (완벽하게 이해 안돼서 일단 넘어감)