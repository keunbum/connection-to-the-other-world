# 02-1 소켓의 프로토콜과 그에 따른 데이터 전송 특성

(보다 자세한 이해를 위해서는 운영체제를 깊게 파볼 것.)

---

## 프로토콜(Protocol)이란 무엇인가?

<참조>

* [한국어 위키](https://ko.wikipedia.org/wiki/%ED%86%B5%EC%8B%A0_%ED%94%84%EB%A1%9C%ED%86%A0%EC%BD%9C)

**protocol**

1. U  
    외교 의례, 의전, 외교 관습;
2. C 전문 용어  
    (조약의) 초안[원안]; (합의안·조약의) 보충 협약
3. C 컴퓨터  
    프로토콜, 대화에 필요한 통신 규약

-> 서로 데이터를 주고 받기 위해 정의한 약속.

> 통신 프로토콜 또는 통신 규약은 컴퓨터나 원거리 통신 장비 사이에서 메시지를 주고 받는 양식과 규칙의 체계이다. 통신 프로토콜은 신호 체계, 인증, 그리고 오류 감지 및 수정 기능을 포함할 수 있다. 프로토콜은 형식, 의미론, 그리고 통신의 동기 과정 등을 정의하기는 하지만 구현되는 방법은 독립적이다. 이 때문에 프로토콜은 하드웨어 또는 소프트웨어 그리고 때로는 모두를 사용하여 구현되기도 한다.

---

## 소켓의 생성

[socket](https://man7.org/linux/man-pages/man2/socket.2.html)

```c
#include <sys/socket.h>

int socket(int domain, int type, int protocol);
// -> 성공 시 파일 디스크립터, 실패 시 -1 반환
```

| parameter | role |
|---|---|
| domain | 소켓이 사용할 프로토콜 체계(Protocol Family) 정보 전달 |
| type | 소켓의 데이터 전송 방식에 대한 정보 전달 |
| protocol | 두 컴퓨터 간 통신에 사용되는 프로토콜 정보 전달 |

각각 매개변수에 대해 자세히 살펴보고자 한다.

---

## 프로토콜 체계(Protocol Family)

소켓이 통신에 사용하는 프로토콜은 부류가 나뉜다. 이러한 부류 정보를 가리켜 '프로토콜 체계'라 하며,
프로토콜 체계의 종류는 다음과 같다.

| Name | Purpose | Man page |
|---|---|---|
| AF_UNIX | Local communication | unix(7) |
| AF_LOCAL | Synonym for AF_UNIX |  |
| AF_INET | IPv4 Internet protocols | ip(7) |
| AF_AX25 | Amateur radio AX.25 protocol | ax25(4) |
| AF_IPX | IPX - Novell protocols |  |
| AF_APPLETALK AppleTalk | ddp(7) |  |
| AF_X25 | ITU-T X.25 / ISO-8208 protocol | x25(7) |
| AF_INET6 | IPv6 Internet protocols | ipv6(7) |
| AF_DECnet | DECet protocol sockets |  |
| AF_KEY | Key management protocol, originally |  |
| developed for usage with IPsec |  |  |
| AF_NETLINK | Kernel user interface device | netlink(7) |
| AF_PACKET | Low-level packet interface | packet(7) |
| AF_RDS | Reliable Datagram Sockets (RDS) protocol | rds(7) |
| rds-rdma(7) |  |  |
| AF_PPPOX | Generic PPP transport layer, for setting |  |
| up L2 tunnels (L2TP and PPPoE) |  |  |
| AF_LLC | Logical link control (IEEE 802.2 LLC) |  |
| protocol |  |  |
| AF_IB | InfiniBand native addressing |  |
| AF_MPLS | Multiprotocol Label Switching |  |
| AF_CAN | Controller Area Network automotive bus |  |
| protocol |  |  |
| AF_TIPC | TIPC, "cluster domain sockets" protocol |  |
| AF_BLUETOOTH Bluetooth low-level socket protocol |  |  |
| AF_ALG | Interface to kernel crypto API |  |
| AF_VSOCK | VSOCK (originally "VMWare VSockets") | vsock(7) |
| protocol for hypervisor-guest |  |  |
| communication |  |  |
| AF_KCM | KCM (kernel connection multiplexer) |  |
| interface |  |  |
| AF_XDP | XDP (express data path) interface |  |

(더 자세한 건 sys/socket.h 헤더 파일 참조)

앞으로는 PF_INET에 해당하는 프로토콜에 체계에 초점을 맞춰 설명을 진행한다.
실제 소켓이 사용할 최종 프로토콜 정보는 socket 함수의 세 번째 인자를 통해서 전달하게 되어 있다.
단, 첫 번째 인자를 통해서 지정한 프로토콜 체계의 범위 내에서 세 번째 인자가 결정되어야 한다.

---

## 소켓의 타입(Type)

소켓의 타입이란 소켓의 데이터 전송 방식을 의미한다. 프로토콜 체계가 결정되었다고 해서 데이터의 전송 방식까지
완전히 결정되는 것은 아니다. PF_INET에 해당하는 프로토콜 체계에도 둘 이상의 데이터 전송 방식이 존재한다.  
이어서 대표적인 데이터 전송 방식 두 가지를 설명한다.

---

## 소켓의 타입 1: 연결지향형 소켓(SOCK_STREAM)

![conveyor](https://media.istockphoto.com/id/875013748/es/foto/trabajadores-que-trabajan-en-banda-transportadora-en-planta-de-envasado.jpg?s=612x612&w=0&k=20&c=3LAP6dn9XpsiliGCtCbDGXkw_au5cthRcU8uUv68BpI=)

연결지향형 소켓은 두 사람이 하나의 라인을 통해 물건을 주고 받는 상황에 비유할 수 있다. 양방향 동시 전송 방식이라는 점에 유의해야 한다.

* 중간에 데이터가 소멸되지 않고 목적지로 전송된다.
* 전송 순서대로 데이터가 수신된다.
* 전송되는 데이터의 경계(Boundary)가 존재하지 않는다.
* 소켓 대 소켓의 연결은 반드시 1대1 이어야 한다.

데이터를 송수신하는 소켓은 내부적으로 버퍼(buffer), 쉽게 말해 바이트 배열을 지니고 있다. 그리고 소켓을 통해 전송되는 데이터는 일단 이 배열에 저장된다. 때문에 데이터가 수신되었다고 해서 곧바로 read 함수를 호출할 필요는 없다. 모든 데이터가 채워진 후에 한 번의 read 함수 호출을 통해 데이터를 전부 읽어 들일 수도 있고, 버퍼가 작다면 여러 번의 호출을 통해 읽어들일 수도 있다. 이런 특성 때문에 연결지향성 소켓은 데이터의 경계가 존재하지 않는다고 말하는 것이다.

>🔖 **소켓에 존재하는 버퍼가 꽉 차면 데이터가 소멸되나요?**  
> read 함수 호출로 읽혀지는 데이터의 양보다 많은 양의 데이터가 수신되면 버퍼도 꽉 찰 수 있다. 그리고 이 상태에 놓인 소켓은 더 이상 데이터를 수신할 수 없다. 하지만 이 상황에 놓여도 전송되는 데이터가 소멸되는 일은 발생하지 않는다. 데이터를 전송하는 영역의 소켓이 더 이상 데이터를 전송하지 않기 때문이다. 즉, 연결 지향형 소켓은 자신과 연결된 상대 소켓의 상태를 파악해가며 데이터를 전송한다. 혹 데이터가 제대로 전송되지 않으면 데이터를 재전송하기까지 한다. 따라서 연결 지향형 소켓의 데이터 손실은 특별한 경우가 아니면 발생하지 않는다.

---

## 소켓의 타입 2: 비 연결지향형 소켓(SOCK_DGRAM)

socket 함수의 두 번째 인자로 SOCK_DGRAM을 전달하면 '비 연결 지향형 소켓'이 생성된다.

* 전송된 순서에 상관없이 가장 빠른 전송을 지향한다.
* 전송된 데이터는 손실이나 훼손의 우려가 있다.
* 전송되는 데이터의 경계(Boundary)가 존재한다.
* 한번에 전송할 수 있는 데이터의 크기가 제한된다.

데이터의 경계가 존재한다는 것은 데이터를 전송할 때 두 번의 함수 호출이 수반되었다면, 데이터를 수신할 때에도 두 번의 함수 호출이 수반되어야 함을 의미한다.

---

## 프로토콜의 최종 선택!

이제 socket 함수의 세 번째 인자에 대해 살펴볼 차례. 이는 최종적으로 소켓이 사용하게 될 프로토콜 정보를 전달하는 목적으로 존재한다. 사실 앞서 언급한 두 개의 인자 정도면 프로토콜 결정에 충분한 정보가 주어진 것 아닌가 하는 생각이 들 수 있다.

맞다. 대부분의 경우 세 번째 인자로 그냥 0을 넘겨줘도 우리가 원하는 소켓을 생성할 수 있다. 하지만 '하나의 프로토콜 체계 안에 데이터 전송 방식이 동일한 프로토콜이 둘 이상 존재하는 경우'가 있기에 세 번째 인자는 필요하다. 

---

## 연결지향형 소켓! TCP 소켓의 예

https://github.com/keunbum/connection-to-the-other-world/blob/fecef46411caedf73520ecebf474a3c7e319fb95/chapters/chapter02/01/tcp_server.c#L1-L65

https://github.com/keunbum/connection-to-the-other-world/blob/fecef46411caedf73520ecebf474a3c7e319fb95/chapters/chapter02/01/tcp_client.c#L1-L60

위 소스 코드는 연결 지향형 소켓인 TCP 소켓의 특성을 파악하기 위한 예제이다.

    "전송되는 데이터의 경계(Boundary)가 존재하지 않는다."

이를 확인하기 위해서는 서버의 write 함수의 호출횟수와 클라이언트의 read 함수의 호출횟수를 불일치 시켜보면 된다.