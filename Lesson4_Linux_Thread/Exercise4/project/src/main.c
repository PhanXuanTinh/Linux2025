#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

#define ARRAY_SIZE 100

int numbers[ARRAY_SIZE];
int even_count = 0;
int odd_count = 0;

// Thread count even
void* count_even(void* arg) {
    int count = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (numbers[i] % 2 == 0) {
            count++;
        }
    }
    even_count = count;
    pthread_exit(NULL);
}

// Thread count odd
void* count_odd(void* arg) {
    int count = 0;
    for (int i = 0; i < ARRAY_SIZE; i++) {
        if (numbers[i] % 2 != 0) {
            count++;
        }
    }
    odd_count = count;
    pthread_exit(NULL);
}

int main()
{
    pthread_t even_thread, odd_thread;
    srand(time(NULL));

    for (int i = 0; i < ARRAY_SIZE; i++) {
        numbers[i] = rand() % 100 + 1;
    }

    pthread_create(&even_thread, NULL, count_even, NULL);
    pthread_create(&odd_thread, NULL, count_odd, NULL);

    pthread_join(even_thread, NULL);
    pthread_join(odd_thread, NULL);

    printf("Total even numbers: %d\n", even_count);
    printf("Total odd numbers: %d\n", odd_count);

    pthread_exit(NULL);
    return 0;
}
