#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_READERS 5
#define NUM_WRITERS 2
#define NUM_ITERATIONS 10

int data = 0;
pthread_rwlock_t rwlock;

//Reader thread function
void* reader(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_rwlock_rdlock(&rwlock); 
        printf("Reader %ld: read data = %d\n", (long)arg, data);
        pthread_rwlock_unlock(&rwlock); 
        usleep(100); 
    }
    pthread_exit(NULL);
}

//Writer thread function
void* writer(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        pthread_rwlock_wrlock(&rwlock);
        data++; 
        printf("Writer %ld: wrote data = %d\n", (long)arg, data);
        pthread_rwlock_unlock(&rwlock); 
        usleep(200); 
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    pthread_rwlock_init(&rwlock, NULL);

    for (long i = 0; i < NUM_READERS; i++) {
        pthread_create(&readers[i], NULL, reader, (void*)i);
    }

    for (long i = 0; i < NUM_WRITERS; i++) {
        pthread_create(&writers[i], NULL, writer, (void*)i);
    }

    for (int i = 0; i < NUM_READERS; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < NUM_WRITERS; i++) {
        pthread_join(writers[i], NULL);
    }

    printf("Final value of data: %d\n", data);

    pthread_rwlock_destroy(&rwlock);
    pthread_exit(NULL);
    return 0;
}
