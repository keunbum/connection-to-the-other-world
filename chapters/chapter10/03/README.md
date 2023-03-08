# 10-3 시그널 핸들링

---

## 운영체제야! 네가 좀 알려줘


---

## 시그널과 signal 함수

[signal](https://man7.org/linux/man-pages/man2/signal.2.html)

근데 이거 사용하지 말란다. 리눅스 버전에 따라 동작이 다른가봄.

```c
#include <signal.h>

void (*signal(int signo, void (*func)(int)))(int);

// It can also be understood as follows.
typedef void (*signalhandler_t)(int);
signalhandler_t signal(int signum, sighandler_t handler);
```

[alarm](https://man7.org/linux/man-pages/man2/alarm.2.html)

한 가지 중요한 사실!
    
    "시그널이 발생하면 sleep 함수의 호출로 블로킹 상태에 있던 프로세스가 깨어난다."

---

## sigaction 함수를 이용한 시그널 핸들링

[sigaction](https://man7.org/linux/man-pages/man2/sigaction.2.html)

The sigaction structure is defined as something like:

```c
    struct sigaction {
        void     (*sa_handler)(int);
        void     (*sa_sigaction)(int, siginfo_t *, void *);
        sigset_t   sa_mask;
        int        sa_flags;
        void     (*sa_restorer)(void);
    };

```

## 시그널 핸들링을 통한 좀비 프로세스의 소멸

자식 프로세스가 종료된 상황에 대한 시그널은 `SIGCHLD`.