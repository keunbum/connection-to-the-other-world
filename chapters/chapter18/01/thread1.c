#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

void* thread_main(void*);

int main(int argc, char* argv[])
{
    int iter_num = 3;
    pthread_t tid;
    if (pthread_create(&tid, NULL, thread_main, &iter_num) != 0) {
        return 1;
    }
    sleep(5);
    puts("end of main");
    return 0;
}

void* thread_main(void* arg)
{
    for (int i = 0; i < *(int*)arg; ++i) {
        sleep(1);
        puts("running thread");
    }
    return NULL;
}