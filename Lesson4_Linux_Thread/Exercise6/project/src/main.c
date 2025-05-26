#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define ARRAY_SIZE 1000000
#define NUM_THREADS 4

int numbers[ARRAY_SIZE];
long long total_sum = 0;
pthread_mutex_t sum_mutex;

typedef struct {
    int start_index;
    int end_index;
} ThreadData;

//Thread function to calculate the sum of a portion of the array
void* partial_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long partial = 0;
    for (int i = data->start_index; i < data->end_index; i++) {
        partial += numbers[i];
    }

    pthread_mutex_lock(&sum_mutex);
    total_sum += partial;
    pthread_mutex_unlock(&sum_mutex);
    pthread_exit(NULL);
}

int main()
{
    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    int segment_size = ARRAY_SIZE / NUM_THREADS;

    // Initialize an array of integers from 1 to 1 million.
    for (int i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = 1; 
    }

    pthread_mutex_init(&sum_mutex, NULL);

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_data[i].start_index = i * segment_size;
        thread_data[i].end_index = (i == NUM_THREADS - 1) ? ARRAY_SIZE : (i + 1) * segment_size;
        pthread_create(&threads[i], NULL, partial_sum, &thread_data[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total sum of array: %lld\n", total_sum);

    pthread_mutex_destroy(&sum_mutex);
    pthread_exit(NULL);

    return 0;
} 