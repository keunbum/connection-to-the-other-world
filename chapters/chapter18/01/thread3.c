#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <unistd.h>

#define THREAD_NUM (2)

void* thread_main(void*);

pthread_mutex_t mutex;

static long long g_sum = 0LL;
// static atomic_llong g_sum = 0;

typedef struct {
    pthread_t thread_id;
} worker_t;

int main(int argc, char* argv[])
{
    pthread_mutex_init(&mutex, NULL);
    worker_t workers[THREAD_NUM];
    for (int i = 0; i < THREAD_NUM; ++i) {
        if (pthread_create(&(workers[i].thread_id), NULL, thread_main, (void*)(5 * i + 1)) != 0) {
            puts("pthread_create() error");
            return 1;
        }
    }
    for (int i = 0; i < THREAD_NUM; ++i) {
        int res;
        if ((res = pthread_join(workers[i].thread_id, NULL)) != 0) {
            errno = res;
            perror("error!!");
            return 1;
        }
    }
    printf("sum = %lld\n", g_sum);
    assert(g_sum == 55);
    return 0;
}

void* thread_main(void* arg)
{
    pthread_mutex_lock(&mutex);
    int start = (int)arg;
    printf("start: %d\n", start);
    for (int i = start; i < start + 5; ++i) {
        printf("i: %d\n", i);
        g_sum += i;
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}