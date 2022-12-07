# 03-5 윈도우 기반으로 구현하기

지금까지 설명한 구조체 sockaddr_in이나, 여러 가지 변환함수들이 윈도우에서 동일한 이름으로 존재하며, 사용방법과 의미까지 동일하다. 따라서 특별히 윈도우 기반이라고 해서 많은 부분은 수정하거나 다른 함수를 가져다 쓸 필요가 없다. 그럼 앞서 보인 몇몇 프로그램을 윈도우 기반으로 수정해 보이겠다.

---

## 함수 htons, htonl의 윈도우 기반 사용 예

리눅스에서와 큰 차이 X.  
자세한 건 [endian_conv_win.c](endian_conv_win.c) 파일 참조.

---


## 함수 inet_addr, inet_ntoa의 윈도우 기반 사용 예

윈도우에는 inet_aton 함수가 존재하지 않으므로 이에 대한 예는 생략하였다.  
-> 과연 [정말로](https://learn.microsoft.com/en-us/windows/win32/api/ws2tcpip/nf-ws2tcpip-inet_pton) [없을까..?](https://learn.microsoft.com/ko-kr/windows/win32/api/ws2tcpip/nf-ws2tcpip-inetptonw)  

그리고 최신 윈도우에선 [RtlIpv4StringToAddress](https://learn.microsoft.com/en-us/windows/win32/api/ip2string/nf-ip2string-rtlipv4stringtoaddressa)를 사용한다고 한다.  

[inet_adrconv_win.c](inet_adrconv_win.c) 파일 참조


---



## 윈도우에서 소켓에 인터넷 주소 할당하기

변수 이름 형식만 다르고 리눅스 기반과 차이가 없다.

---


## WSAStringToAddress & WSAAddressToString

이번에는 윈속2에서 추가된 변환함수 둘을 소개하겠다. 이 둘은 inet_ntoa, 그리고 inet_addr 함수와
기능은 같으나 다양한 프로토콜에 적용이 가능하다는 장점이 있다. 즉 IPv4뿐 아니라 IPv6에서도 사용
이 가능하다. 물론 장점만 있는 것은 아니다. inet_ntoa, inet_addr 함수를 사용할 경우 리눅스 기반
에서 윈도우 기반으로, 그리고 그 반대로도 프로그램의 변경이 용이하지만, 다음 두 함수를 사용하면 윈
도우에 종속적인 코드가 만들어지기 때문에 다른 운영체제로의 이식성이 떨어진다는 단점이 있다. 때문
에 이 책에서는 다음 두 함수를 사용하지 않는다. 다양한 함수를 소개한다는 측면에서 언급만할 뿐이다.  

### WSAStringToAddress

주소정보를 나타내는 문자열을 가지고 주소정보 구조체 변수를 적절히 채워 넣을 때 호출하는 함수.

```c
#include <winsock2.h>

INT WSAAPI WSAStringToAddressA(
  [in]           LPSTR               AddressString,
  [in]           INT                 AddressFamily,
  [in, optional] LPWSAPROTOCOL_INFOA lpProtocolInfo,
  [out]          LPSOCKADDR          lpAddress,
  [in, out]      LPINT               lpAddressLength
);
// ->  성공 시 0, 실패 시 SOCKET_ERROR 반환
```

| parameger       | role                                                           |
|-----------------|----------------------------------------------------------------|
| AddressString   | IP와 PORT번호를 담고 있는 문자열의 주소 값                        |
| AddressFamily   | 첫 번째 인자로 전달된 주소 정보가 속하는 주소체계 정보전달          |
| lpProtocolInfo  | 프로토콜 프로바이더(Provider) 설정, 일반적으로 NULL 전달           |
| lpAddress       | 주소정보를 담을 구조체 변수의 주소 값                              |
| lpAddressLength | 네 번째 인자로 전달된 주소 값의 변수 크기를 담고 있는 변수의 주소 값 |


### WSAAddressToString

구조체 변수에 저장된 데이터를 참조하여 주소정보를 담고 있는 문자열을 만들어 반환한다.

```c
#include <winsock2.h>

INT WSAAPI WSAAddressToStringA(
  [in]           LPSOCKADDR          lpsaAddress,
  [in]           DWORD               dwAddressLength,
  [in, optional] LPWSAPROTOCOL_INFOA lpProtocolInfo,
  [in, out]      LPSTR               lpszAddressString,
  [in, out]      LPDWORD             lpdwAddressStringLength
);
// -> 성공 시 0, 실패 시 SOCKET_ERROR 반환
```

| parameter               | role                                                            |
|-------------------------|-----------------------------------------------------------------
| lpsaAddress             | 문자열로 변환할 주소정보를 지니는 구조체 변수의 주소 값             |
| dwAddressLength         | 첫 번째 인자로 전달된 구조체 변수의 크기                           |
| lpProtocolInfo          | 프로토콜 프로바이더(Provider) 설정, 일반적으로 NULL 전달           |
| lpszAddressString       | 문자열로 변환된 결과를 저장할 배열의 주소 값                       |
| lpdwAddressStringLength | 네 번째 인자로 전달된 주소 값의 배열 크기를 담고 있는 변수의 주소 값 |


### Example

```c
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#undef UNICODE
#undef _UNICODE

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

	{
		char* strAddr = "203.211.218.102:9190";
		char strAddrBuf[50];
		SOCKADDR_IN servAddr;
		int addrLength;

		addrLength = sizeof(servAddr);
		WSAStringToAddress(
			strAddr, AF_INET, NULL, (SOCKADDR*)&servAddr, &addrLength);

		addrLength = sizeof(strAddrBuf);
		WSAAddressToString(
			(SOCKADDR*)&servAddr, sizeof(servAddr), NULL, strAddrBuf, &addrLength);

		printf("Second conv result: %s\n", strAddrBuf);
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

>🔖 **UNICODE와 _UNICODE**  
>#undef는 기존에 정의된 매크로를 해제하는 경우에 사용한다. 프로젝트의 환경에 따라서 VC++ 자체적으로 이 두 매크로를 정의하는 경우가 있는데, 그렇게 되면 두 함수의 매개변수형이 유니코드 기반으로 바뀌어서 원치 않은 실행 결과를 보이게 된다. 그래서 이와 같은 문장을 삽입하였다.
