//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>

int main(int argc, char* argv[]) {
    uint32_t threads_num = 4;

    // set the number of threads
    omp_set_num_threads(threads_num);

    #if SCHEDULE == 0 
    #pragma omp parallel for
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 1
    #pragma omp parallel for schedule(static)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 2
    #pragma omp parallel for schedule(static,1)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 3
    #pragma omp parallel for schedule(static,4)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 4
    #pragma omp parallel for schedule(dynamic,1)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 5
    #pragma omp parallel for schedule(dynamic,4)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 6
    #pragma omp parallel for schedule(guided,1)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    #if SCHEDULE == 7
    #pragma omp parallel for schedule(guided,4)
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }
    #endif

    return 0;
}