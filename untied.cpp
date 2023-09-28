//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
#include <unistd.h>

#define LARGE_NUMBER 1000000

extern void UslessTaskFunction(int task_id) {
    printf("Out: %d\n", task_id);
}

extern void PotentialDangerousUslessTaskFunction(int task_id, int thead_rank) {
    printf("Task [%d] Thread rank: %d\n", task_id, thead_rank);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[-] Usage %s n -- the number of threads\n", argv[0]);
        return -1;
    }

    uint32_t threads_num = atoi(argv[1]);

    // set the number of threads
    omp_set_num_threads(threads_num);
    // start the parallel section
    #pragma omp parallel
    {
        #pragma omp single
        {
            int i = 0;
            #pragma omp task untied
            {
                for (i = 0; i < LARGE_NUMBER; i++) {
                    printf("Task %d was born by thread %d\n", i, omp_get_thread_num());
                    #pragma omp task
                    {
                        PotentialDangerousUslessTaskFunction(i, omp_get_thread_num());
                    }
                }
            }
        }
    }

    return 0;
}