#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define NUM_ITERATIONS 10

int data;
pthread_mutex_t data_mutex; // Mutex
pthread_cond_t data_cond;   // Condition variable

//Producer
void* producer(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        int random_number = rand() % 10 + 1;
        // Lock the mutex before changing the data.
        pthread_mutex_lock(&data_mutex);
        data = random_number; // update data
        printf("Producer produced: %d\n", data);
        // Signal to the consumer that the data is ready.
        pthread_cond_signal(&data_cond);
        pthread_mutex_unlock(&data_mutex); // unlock mutex

        sleep(1);
    }
    pthread_exit(NULL);
}

void* consumer(void* arg) {
    for (int i = 0; i < NUM_ITERATIONS; i++) {
        // Lock the mutex before read the data.
        pthread_mutex_lock(&data_mutex);
        // Wait until the producer provides the data.
        pthread_cond_wait(&data_cond, &data_mutex);
        printf("Consumer consumed: %d\n", data);
        pthread_mutex_unlock(&data_mutex); // unlock mutex
    }
    pthread_exit(NULL);
}

int main()
{
    pthread_t producer_thread, consumer_thread;

    pthread_mutex_init(&data_mutex, NULL);
    pthread_cond_init(&data_cond, NULL);

    srand(time(NULL));

    pthread_create(&producer_thread, NULL, producer, NULL);
    pthread_create(&consumer_thread, NULL, consumer, NULL);

    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    pthread_mutex_destroy(&data_mutex);
    pthread_cond_destroy(&data_cond);

    pthread_exit(NULL);
    return 0;
}
