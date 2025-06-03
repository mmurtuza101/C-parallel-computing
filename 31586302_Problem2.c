#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define PI 3.14159265358979323846264338327950288419716939937510

long double pi = 0.0;
pthread_mutex_t mutex;
unsigned long global_n;          
unsigned long global_num_threads; 

void* calculate_pi(void* arg) {
    unsigned long thread_id = (unsigned long)arg;
    unsigned long start = thread_id * (global_n / global_num_threads);
    unsigned long end = (thread_id + 1) * (global_n / global_num_threads);

    if (thread_id == global_num_threads - 1) {
        end = global_n;
    }

    long double partial_sum = 0.0;
    for (unsigned long i = start; i < end; i++) {
        double x = ((double)i - 0.5) / global_n;
        partial_sum += 4.0 / (1.0 + x * x);
    }

    pthread_mutex_lock(&mutex);
    pi += partial_sum;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: %s <num_terms> <num_threads>\n", argv[0]);
        return 1;
    }

    global_n = atol(argv[1]);
    global_num_threads = atoi(argv[2]);
    pthread_t threads[global_num_threads];
    pthread_mutex_init(&mutex, NULL);

    for (unsigned long i = 0; i < global_num_threads; i++) {
        pthread_create(&threads[i], NULL, calculate_pi, (void*)i);
    }

    for (int i = 0; i < global_num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    pi /= global_n;
    long double error = fabsl(pi - PI);
    printf("Pi is approximately %.22Lf, Error is %.22Lf\n", pi, error);

    pthread_mutex_destroy(&mutex);
    return 0;
}
