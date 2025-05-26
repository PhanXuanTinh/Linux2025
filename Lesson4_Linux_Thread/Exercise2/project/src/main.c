#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 3
#define INCREMENTS 1000000

long counter = 0;
pthread_mutex_t counter_mutex;

void* increment_counter(void* thread_id) {
    for (int i = 0; i < INCREMENTS; i++) {
        pthread_mutex_lock(&counter_mutex); // lock mutex
        counter++; // Tăng giá trị của counter
        pthread_mutex_unlock(&counter_mutex); // unlock mutex
    }
    pthread_exit(NULL); // Kết thúc thread
}

int main()
{
    pthread_t threads[NUM_THREADS]; 
    int rc;

    pthread_mutex_init(&counter_mutex, NULL);

    for (long t = 0; t < NUM_THREADS; t++) {
        printf("Main: creating thread %ld\n", t);
        rc = pthread_create(&threads[t], NULL, increment_counter, (void*)t);
        if (rc) {
            printf("Error: Unable to create thread, %d\n", rc);
            exit(-1);
        }
    }

    // Wait for all threads to complete
    for (long t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    printf("Final counter value: %ld\n", counter);

    // Release mutex
    pthread_mutex_destroy(&counter_mutex);
    pthread_exit(NULL);
    return 0;
}
