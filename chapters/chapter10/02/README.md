# 10-2 프로세스 & 좀비(Zombie) 프로세스

---

## 좀비(Zombie) 프로세스

---

## 좀비 프로세스의 생성 이유
다음과 같이 후면 처리를 하면 터미널을 따로 열지 않아도 된다.
```bash
$ ./zombie &
```

---

## 좀비 프로세스의 소멸1: wait 함수의 사용

[wait](https://man7.org/linux/man-pages/man2/wait.2.html)

wait()은 자식 중 하나가 종료될 때까지 호출 스레드 실행을 일시 중지한다.

* WIFEXITED()     자식 프로세스가 정상 종료한 경우 '참(true)'을 반환한다.
* WEXITSTATUS()   자식 프로세스의 전달 값을 반환한다.

---

## 좀비 프로세스의 소멸2: waitpid 함수의 사용

[waitpid](https://man7.org/linux/man-pages/man2/wait.2.html)

wait 함수의 블로킹이 우려된다면 waitpid가 그 대안이다.
좀비 프로세스를 막을 수도 있고, 블로킹을 해결할 수도 있다.

`wait(&wstatus)`는 `waitpid(-1, &wstatus, 0)`과 같다.

성공 시 종료된 자식 프로세스의 ID(또는 0), 실패 시 -1 반환.
`options`에 `WNOHANG`을 전달하면, 종료된 자식 프로세스가 없어도
0을 반환하면서 함수를 빠져 나온다.

