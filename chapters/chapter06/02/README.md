# 06-2 UDP 기반 서버/클라이언트의 구현

책 내용에 더하여 내면화를 위한 추가 기술도 있음.  
암기하지 말고 이해하자. 암기하면 또 까먹는다...

TCP/IP와 비교/대조 해 가면서 공부하자.
그렇기에 TCP/IP에 대해서 제대로 이해하고 있어야 한다.

이제 구현 관점에서 살펴 보자.

---

## UDP에서의 서버와 클라이언트는 연결되어 있지 않습니다.

---

## UDP에서는 서버건 클라이언트건 하나의 소켓만 있으면 됩니다.

소켓 하나로 둘 이상의 호스트와 통신 가능.

---

## UDP 기반의 데이터 입출력 함수

UDP 소켓은 데이터를 전송할 때마다 목적지 주소정보를 별도로 추가해야 한다.

* [sendto die.net](https://linux.die.net/man/2/sendto)
* [sendto man page](https://man7.org/linux/man-pages/man3/sendto.3p.html)

* [recvfrom joinc](https://www.joinc.co.kr/w/man/2/recvfrom)
* [recvfrom die.net](https://linux.die.net/man/2/recvfrom)
* [recvfrom wariua](https://wariua.github.io/man-pages-ko/recvfrom%282%29/)
* [recvfrom man page](https://man7.org/linux/man-pages/man3/recvfrom.3p.html)

---

## UDP 기반의 에코 서버와 에코 클라이언트

UDP에서는 TCP와 달리 연결 요청과 그에 따른 수락의 과정이 존재하지 않기 때문에  
서버 밑 클라이언트라는 표현이 적절치 않은 부분도 있다.  
다만 서비스를 제공한다는 측면에서 서버라 하는 것일 뿐.

---

## UDP 클라이언트 소켓의 주소 정보 할당

TCP는 connect 때 자동 호출..

UDP도 bind를 호출할 수 있으나, sendto 때 자동으로 할당되기에 대개 생략하는 편.