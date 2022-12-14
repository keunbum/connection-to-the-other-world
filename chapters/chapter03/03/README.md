# 03-3 네트워크 바이트 순서와 인터넷 주소 변환

CPU에 따라서 4바이트 정수 1을 메모리공간에 저장하는 방식이 달라질 수 있음을 알고 있는가? 4바이트 정수 1을 2진수로 표현하면 다음과 같다.

    00000000 00000000 00000000 00000001

이 순서 그대로 메모리에 저장하는 CPU가 있는가 하면, 다음과 같이 거꾸로 저장하는 CPU도 있다.

    00000001 00000000 00000000 00000000

이러한 부분을 고려하지 않고서 데이터를 송수신하면 문제가 발생할 수 있다. 저장순서가 다르다는 것은 전송되어온 데이터의 해석순서가 다름을 뜻하기 때문이다.

---

## 바이트 순서(Order)와 네트워크 바이트 순서

CPU가 데이터를 메모리에 저장하는 방식은 두 가지로 나뉜다. 이는 CPU가 데이터를 해석하는 방식도 두 가지로 나뉨을 의미한다.

* 빅 엔디안(Big Endian)&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;상위 바이트의 값을 작은 번지수에 저장하는 방식
* 리틀 엔디안(Little Endian)&nbsp;&nbsp;&nbsp;&nbsp;하위 바이트의 값을 작은 번지수에 저장하는 방식

예를 들어 0x10번지를 시작으로 4바이트 int형 정수 0x12345678을 저장한다면 빅 엔디안 CPU는 다음과 같이 메모리에 저장한다.


<img src = "../../../img/%EB%B9%85%20%EC%97%94%EB%94%94%EC%95%88%20%EB%B0%94%EC%9D%B4%ED%8A%B8%20%ED%91%9C%ED%98%84.png" width="30%" height="30%">

<center>[빅 엔디안 바이트 표현]</center>  
  
  
반면 리틀 엔디안 방식은 다음과 같이 저장된다. 이때 1바이트 단위로는 값이 뒤집히지 않음을 알 수 있다.
  
<img src = "../../../img/%EB%A6%AC%ED%8B%80%20%EC%97%94%EB%94%94%EC%95%88%20%EB%B0%94%EC%9D%B4%ED%8A%B8%20%ED%91%9C%ED%98%84.png" width="30%" height="30%">

<center>[리틀 엔디안 바이트 표현]</center>


이렇듯 CPU의 데이터 저장방식을 의미하는 '호스트 바이트 순서(Host Byte Order)'는 CPU에 따라서 차이가 난다. (참고로 인텔 계열 CPU는 리틀 엔디안 방식으로 데이터를 저장한다.) 그래서 호스트 바이트 순서가 다른 두 CPU가 데이터를 주고 받으면 문제가 생기는 것이다.

이러한 문제을 해결하기 위해 네트워크를 통해서 데이터를 전송할 때에는 통일된 기준으로 데이터를 전송하기로 약속하였으며, 이 약속을 가리켜 '네트워크 바이트 순서(Network Byte Order)'라 한다.

    "빅 엔디안으로 통일"

모든 컴퓨터는 수신된 데이터가 네트워크 바이트 순서로 정렬되어 있음을 인지해야 하며, 리틀 엔디안 시스템에서는 데이터를 전송하기에 앞서 빅 엔디안 방식으로 데이터를 재정렬해야 한다.

---


## 바이트 순서의 변환(Endian Conversions)

바이트 순서의 변환을 돕는 함수를 살펴 보자

* uint16_t htons(uint16_t hostshort);
* uint16_t ntohs(uint16_t netshort);
* uint32_t htonl(uint32_t hostlong);
* uint32_t ntohl(uint32_t netlong);

여기서 h는 호스트(host) 바이트 순서, n은 네트워크(network) 바이트 순서를 의미한다.
그리고 s는 short, l은 long을 의미한다.(리눅스 에서 long형은 4바이트이다.)  
일반적으로 뒤에 s가 붙는 함수는 2바이트(16비트)를 의미하므로 PORT번호 변환에 사용되고, l이 붙는 함수는 4바이트(32비트)를 의미하므로 IP주소 변환에 사용된다.  

endian_conv.c 파일을 컴파일하고 실행해보면 다음과 같이 출력된다. (리틀 엔디안 CPU이므로)

```bash
$ ./endian_conv
Host ordered port: 0x1234
Network ordered port: 0x3412
Host ordered address: 0x12345678
Network ordered address: 0x78563412
```

>🔖 **데이터도 전송 전후로 다 바꿔줘야 하나?**  
> 데이터 송수신 시에는 네트워크 바이트 순서와 호스트 바이트 순서 간에 변환이 자동으로 이뤄진다고 한다. 따라서 sockaddr_in 구조체 변수에 데이터를 채울 때 이외에는 바이트 순서를 신경 쓰지 않아도 된다.