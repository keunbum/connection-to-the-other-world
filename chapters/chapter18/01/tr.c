#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>

#define NUM_THREADS 100
#define UPPER_LIMIT 50000000

atomic_llong g_sum = 0;

void* Runner(void* param);

int main(int argc, char* argv[])
{
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;
    int i;
    int rc;

    rc = pthread_attr_init(&attr);
    if (rc != 0) {
        fprintf(stderr, "Error: pthread_attr_init failed\n");
        exit(EXIT_FAILURE);
    }

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_create(&tid[i], &attr, Runner, (void*)(i * (UPPER_LIMIT / NUM_THREADS) + 1));
        if (rc != 0) {
            fprintf(stderr, "Error: pthread_create failed\n");
            exit(EXIT_FAILURE);
        }
    }

    for (i = 0; i < NUM_THREADS; i++) {
        rc = pthread_join(tid[i], NULL);
        if (rc != 0) {
            fprintf(stderr, "Error: pthread_join failed\n");
            exit(EXIT_FAILURE);
        }
    }

    printf("sum = %lld\n", g_sum);
    return 0;
}

void* Runner(void* param)
{
    int i;
    int upper = (int)param;
    for (i = upper; i < upper + (UPPER_LIMIT / NUM_THREADS); i++) {
        atomic_fetch_add(&g_sum, i);
    }

    pthread_exit(0);
}