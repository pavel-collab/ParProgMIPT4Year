//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
#include <math.h>
#include <cstring>

int main(int argc, char* argv[]) {
    uint32_t threads_num = 4;

    // set the number of threads
    omp_set_num_threads(threads_num);

    #pragma omp parallel for
    for (int i = 0; i < 65; ++i) {
        printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    }

    // #pragma omp parallel for schedule(static)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    // #pragma omp parallel for schedule(static,1)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    // #pragma omp parallel for schedule(static,4)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    // #pragma omp parallel for schedule(dynamic,1)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    // #pragma omp parallel for schedule(dynamic,4)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    // #pragma omp parallel for schedule(guided,1)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    // #pragma omp parallel for schedule(guided,4)
    // for (int i = 0; i < 65; ++i) {
    //     printf("Thread %d running iteration %d\n", omp_get_thread_num(), i);
    // }

    return 0;
}