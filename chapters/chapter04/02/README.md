# 04-2 TCP 기반 서버, 클라이언트 구현

TCP 기반 서버를 보다 제대로 이해하여 완벽하게 구현해 보자.

---

## TCP 서버에서의 기본적인 함수 호출 순서

아래 그림은 TCP 서버 구현을 위한 기본적인 함수의 호출 순서를 보이고 있다. 대부분의 TCP 서버 프로그램은 이 순서로 구현된다.  

<img src = "../../../img/TCP%20%EC%84%9C%EB%B2%84%EC%9D%98%20%ED%95%A8%EC%88%98%20%ED%98%B8%EC%B6%9C%20%EC%88%9C%EC%84%9C.png" width="30%" height="30%">

제일 먼저 socket 함수의 호출을 통해 소켓을 생성한다. 그리고 주소 정보를 담기 위한 구조체 변수를
선언 및 초기화해서 bind 함수를 호출하여 소켓에 주소를 할당한다. 이제 그 이후의 과정에 대해 설명하겠다.

---

## '연결 요청 대기 상태'로의 진입

bind 함수 호출을 통해서 소켓에 주소까지 할당했다면, 이제 listen 함수 호출을 통해 '연결요청 대기상태'로
들어갈 차례이다. 그리고 listen 함수가 호출되어야 클라이언트가 연결요청을 할 수 있는 상태가 된다.
즉, listen 함수가 호출되어야 클라이언트는 연결요청을 위해서 connect 함수를 호출할 수 있다(그전에
connect 함수 호출하면 오류 발생).

```c
#include <sys/socket.h>

int listen(int sockfd, int backlog);
// -> 성공 시 0, 실패 시 -1 반환
```

| parameter | role                                                                                                                                    |
|-----------|-----------------------------------------------------------------------------------------------------------------------------------------|
| sockfd    | 연결요청 대기상태에 두고자 하는 소켓의 파일 디스크립터 전달. <br> 이 함수의 인자로 전달된 디스크립터의 소켓이 서버 소켓(리스닝 소켓)이 된다. |
| backlog   | 연결요청 대기 큐(Queue)의 크기정보 전달. <br> 5가 전달되면 큐의 크기가 5가 되어 클라이언트의 연결요청을 5개까지 대기시킬 수 있다.            |

여기서 잠시 '연결요청 대기상태'의 의미와 '연결요청 대기 큐'에 대해서 별도 설명을 추가하겠다.
서버가 '연결요청 대기상태'에 있다는 것은 클라이언트가 연결요청을 했을 때 그 연결이 수락될 때까지
연결요청 자체를 대기시킬 수 있는 상태(그래서 큐가 필요하다)에 있다는 것을 의미한다.  

클라이언트의 연결요청도 인터넷을 통해서 흘러 들어오는 일종의 데이터 전송이기 때문에, 이것을
받아들이려면 당연히 소켓이 하나 있어야 한다. 서버 소켓의 역할이 바로 이것이다. 즉, 연결 요청을 맞이하는,
일종의 문지기 또는 문의 역할을 한다고 볼 수 있다.  
클라이언트가 "똑똑.. 저기요 혹시 제가 연결될 수 있을까요?"라고 서버 소켓에 물어보면, 서버 소켓은 "물론입니다. 그런데 지금 시스템이 조금 바쁘니, 대기실에서 번호표 뽑고 기다리시면 준비되는 대로 바로 연결해 드리겠습니다."라고 답하며 클라이언트의 연결요청을 대기실로 안내하는 식이다. listen 함수가 호출되면, 이렇듯 문지기 역할을 하는
서버 소켓이 만들어지고, listen 함수의 두 번째 인자로 전달되는 정수 크기에 해당하는 대기실이 만들어진다. 이 대기실을
가리켜 '연결요청 대기 큐'라 하며, 서버 소켓과 연결요청 대기 큐가 완전히 준비되어서 클라이언트의 연결요청을 받아들일
수 있는 상태를 가리켜 '연결요청 대기상태'라 한다.

---

## 클라이언트의 연결 요청 수락

listen 함수 호출 이후에 클라이언트의 연결요청이 들어왔다면, 들어온 순서대로 연결요청을 수락해야 한다.
연결요청을 수락한다는 것은 클라이언트와 데이터를 주고 받을 수 있는 상태가 됨을 의미한다. 따라서 이러한 상태가 되기
위해 무엇이 필요한지 짐작할 수 있을 것이다. 당연히 소켓이 필요하다! 데이터를 주고 받으러면 소켓이 있어야 하지 않은가? 물론 서버 소켓을 사용하면 되지 않느냐고 물을 수 있다. 하지만 서버 소켓은 문지기이다. 클라이언트와의 데이터 송수신을 위해 얘를 사용하면 문은 누가 지키겠는가? 때문에 소켓을 하나 더 만들어야 한다. 하지만 우리가 소켓을 직접
만들 필요는 없다. accept 함수를 호출하면 소켓이 만들어지고, 이 소켓은 연결요청을 한 클라이언트 소켓과 자동으로 연결되니 말이다.

```c
#include <sys/socket.h>

int accept(int sockfd, struct sockaddr *restrict addr, socklen_t *restrict addrlen);
// -> 성공 시 생성된 소켓의 파일 디스크립터, 실패 시 -1 반환
```

| parameter | role |
|---|---|
| sockfd | 서버 소켓의 파일 디스크립터 전달. |
| addr | 연결요청 한 클라이언트의 주소정보를 담을 변수의 주소 값 전달. <br> 함수호출이 완료되면 인자로 전달된 변수에는 클라이언트의 주소정보가 채워진다. |
| addrlen | 두 번째 매개변수 addr에 전달된 주소의 변수 크기를 바이트 단위로 전달한다. <br> 단, 변수의 주소 값을 전달하는데 이는 함수를 호출한 후 바이트 단위로 계산된 클라이언트의 주소 정보 길이를 받기 위함이다. |

accept 함수는 '연결요청 대기 큐'에서 대기중인 클라이언트의 연결요청을 수락하는 기능의 함수이다. 따라서 accept 함수는 호출 성공 시 데이터 입출력에 사용할 소켓을 생성하고, 그 소켓의 파일 디스크립터를 반환한다. 대기 큐(Queue)에 있는 연결요청을 하나 꺼낸 후, 새로운 소켓을 생성하여 연결요청을 완료하는 것이다. 내부적으로 소켓을 생성한 후, 연결요청을 한 클라이언트 소켓에 연결까지 해준다. 서버에서 별도로 생성한 소켓과 클라이언트 소켓이 직접 연결되었으면 이제 데이터를 주고받는 일만 남았다. 


---

## Hello world 서버 프로그램 리뷰

이제 [hello_server.c](../../chapter01/01/hello_server.c) 소스 코드를 다시 한번 분석하고자 한다.

https://github.com/keunbum/connection-to-the-other-world/blob/5c69b8c761f266055d551c01a70ff9a3ad3051b5/chapters/chapter01/01/hello_server.c#L1-L66

> **코드 분석**
> * 27행: 이 때 생성되는 소켓은 아직 서버 소켓이라 부르기 이른 상태이다.
> * 33~41행: 소켓 주소할당을 위해 구조체 변수를 초기화하고 bind 함수를 호출하고 있다.
> * 43행: 연결요청 대기상태로 들어가기 위해서 listen 함수를 호출하고 있다. 이제야 비로소 27행에서 생성한 소켓을 가리켜 서버 소켓이라 할 수 있겠다.
> * 49행: 대기 큐에서 첫 번째로 대기 중인 연결요청을 참조하여 클라이언트와의 연결을 구성하고, 생성된 소켓의 파일 디스크립터를 반환한다. 참고로 함수가 호출되었을 때 대기 큐가 비어있다면 대기 큐가 찰 때까지(클라이언트의 연결요청이 들어올 때까지) accept 함수는 반환하지 않는다.
> * 55~57행: write 함수를 통해 클라이언트로 데이터를 전송한다. 이제 close 함수 호출 순서도 그 이유가 감이 잡힐 것이다.

---

## TCP 클라이언트의 기본적인 함수 호출 순서

이제 클라이언트 구현 순서에 대해 언급하겠다.

<img src = "../../../img/TCP%20%ED%81%B4%EB%9D%BC%EC%9D%B4%EC%96%B8%ED%8A%B8%EC%9D%98%20%ED%95%A8%EC%88%98%20%ED%98%B8%EC%B6%9C%20%EC%88%9C%EC%84%9C.png" width="30%" height="30%">

서버 구현 과정과 비교해서 차이가 있는 부분은 '연결요청' 과정이다. 클라이언트는 어떻게 연결요청을 할까?

```c
#include <sys/socket.h>

int connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
// -> 성공 시 0, 실패 시 -1 반환
```

| parameter | role |
|---|---|
| sockfd | 클라이언트 소켓 파일 디스크립터 전달. |
| addr | 연결요청 할 서버의 주소정보를 담은 변수의 주소 값 전달. |
| addrlen | 두 번째 매개변수 addr에 전달된 주소의 변수 크기를 바이트 단위로 전달한다. |

클라이언트에 의해서 connect 함수가 호출되면 다음 둘 중 한 가지 상황이 되어야 함수가 반환된다.
* 서버에 의해 연결요청이 접수됨.
* 네트워크 단절 등 오류 상황이 발생하여 연결요청이 중단됨.

여기서 주의할 사항은 위에서 말하는 '연결요청의 접수'는 서버의 accept 함수 호출을 의미하는 것이 아니라는 점이다.
이는 클라이언트의 연결요청 정보가 서버의 연결요청 대기 큐에 등록되었음을 의미한다. 따라서 connect 함수가 반환됐더라도
당장에 서비스가 이뤄지지 않을 수도 있음을 기억해야 한다. 

>🔖 **클라이언트 소켓의 주소정보는 언제..?**  
> 생각해보면 클라이언트 프로그램의 구현순서에는 소켓의 주소할당 과정이 없었다.
그저 소켓을 생성하고 connect 함수를 호출한 것이 전부였는데, 그럼 클라이언트 소켓은 IP와 PORT 할당이 불필요한 것일까?
물론 아니다! 네트워크를 통해서 데이터를 송수시하려면 반드시 IP와 PORT가 할당되어야 하는 법. 그렇다면 클라이언트는 언제,
어디서, 어떻게 할당이 가능했던 것일까?
> * 언제? ------------- connect 함수가 호출될 때
> * 어디서? ----------- 운영체제에서, 더 정확히 말하면 커널에서
> * 어떻게? ----------- IP는 컴퓨터(호스트) 것으로, PORT는 임의로 선택
> 즉 bind 함수를 통해서 소켓에 IP와 PORT를 직접 할당하지 않아도 connect 함수 호출 시 자동으로 소켓에 IP와 PORT가 할당된다. 따라서 클라이언트 프로그램을 작성할 때에는 bind 함수를 명시적으로 호출할 필요가 없다.

---

## Hello world 클라이언트 프로그램 리뷰

앞서 Hello world 서버 프로그램을 리뷰한 것처럼 [hello_client.c](../../chapter01/01/hello_client.c) 소스 코드도 살펴 보자.

https://github.com/keunbum/connection-to-the-other-world/blob/5c69b8c761f266055d551c01a70ff9a3ad3051b5/chapters/chapter01/01/hello_client.c#L1-L55

> **코드 분석**
> * 23행: TCP 소켓 생성.
> * 29~32행: 초기화 값은 연결을 목적으로 하는 서버 소켓의 IP와 PORT 정보이다.
> * 34행: 서버로 연결 요청.
> * 39행: 서버로부터 전송된 데이터 수신.
> * 46행: 소켓 닫기 -> 서버와의 연결 종료.


---

## TCP 기반 서버, 클라이언트의 함수 호출 관계

지금까지 TCP 서버/클라이언트 프로그램 구현순서를 설명했는데, 이 둘은 서로 독립된 과정이 아니기 때문에 하나의 과정으로
머리 속에 그릴 수 있어야 한다.

<img src = "../../../img/TCP%20%EC%84%9C%EB%B2%84-%ED%81%B4%EB%9D%BC%EC%9D%B4%EC%96%B8%ED%8A%B8%20%ED%95%A8%EC%88%98%20%ED%98%B8%EC%B6%9C%20%EA%B4%80%EA%B3%84.png" width="40%" height="40%">

클라이언트가 connect 함수를 호출하기에 앞서 서버가 accept 함수를 먼저 호출할 수 있음을 기억하자. 물론 이때는
클라이언트가 connect 함수를 호출할 때까지 서버는 accept 함수가 호출된 위치에서 블로킹 상태에 놓이게 된다.