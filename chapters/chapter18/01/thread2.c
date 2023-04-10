#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void* thread_main_malloc(void*);

int main(int argc, char* argv[])
{
    pthread_t tid;
    int iter_num = 3;
    if (pthread_create(&tid, NULL, thread_main_malloc, (void*)&iter_num) != 0) {
        puts("error");
        return 1;
    }
    void* retval = NULL;
    if (pthread_join(tid, &retval) != 0) {
        puts("join error");
        return 1;
    }
    printf("Thread return msg: %s\n", (char*)retval);
    free(retval);
    retval = NULL;
    return 0;
}

void* thread_main_malloc(void* arg)
{
    for (int i = 0; i < *(int*)arg; ++i) {
        sleep(1);
        puts("running thread");
    }
    char* pa_msg = (char*)malloc(sizeof(char) * 50);
    strcpy(pa_msg, "Hello, I'am thread~");
    return (void*)pa_msg;
}