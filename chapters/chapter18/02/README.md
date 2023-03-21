# 18-2 스레드의 생성 및 실행

---

## 스레드의 생성과 실행 흐름의 구성

책에서는 컴파일 시 -lpthread를 사용하라고 나와 있지만,  
man page에는 -pthread로 나와 있음.

[restrict](https://en.wikipedia.org/wiki/Restrict)

[pthread_create](https://man7.org/linux/man-pages/man3/pthread_create.3.html)

[pthread_join](https://man7.org/linux/man-pages/man3/pthread_join.3.html)

---


## 임계 영역 내에서 호출이 가능한 함수


~~컴파일시 옵션으로 -DREENTRANT를 붙이면 스레드-불안전한  
라이브러리 함수들을 모두 스레드-안전한 함수들로 바꿔준다.~~

이거 검색해봤더니, 책 내용이 상당히 오래된 것 같다..  
지금은 다 스레드 안전한 함수들 같은데?

책 내용 맹신하면 안될듯.. Windows 함수도 다 구식이더라..   
검색 많이 해봐야 할듯ㅠㅠㅠ.



---


## 워커(Worker) 스레드 모델


---


