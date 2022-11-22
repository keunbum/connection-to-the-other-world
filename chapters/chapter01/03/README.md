# 01-3; 윈도우 기반으로 구현하기

* 윈도우 소켓을 위한 헤더와 라이브러리의 설정

    윈속(윈도우 소켓)을 기반으로 프로그램을 개발하기 위해서는 기본적으로 다음 두 가지를 진행해야 한다.

        * 헤더파일 winsock2.h를 포함한다.
        * ws2_32.lib 라이브러리를 링크시켜야 한다.
        
    <ws2_32.lib 라이브러리 추가하기>
    * '프로젝트 속성' -> '속성 페이지' -> '구성 속성' -> '링커' -> '입력' -> '추가 종속성' -> "ws2_32.lib" 입력
    * 소스 코드 안에 ```#pragma comment(lib, "ws2_32.lib")``` 을 추가해도 좋다.

---

* 윈속(winsock)의 초기화

    윈속 프로그래밍을 할 때에는 반드시 WSAStartup 함수를 호출해서, 프로그램에서 요구하는 윈도우 소켓의 버전을 알리고, 해당 버전을 지원하는 라이브러리의 초기화 작업을 진행해야 한다.

    ```c++
    #include <winsock2.h>

    int WSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);
    ```

    초기화된 라이브러리의 해제.
    대개 프로그램이 종료되기 직전에 호출.

    ```c++
    #include <winsock2.h>

    int WSACleanup(void);
    ```
