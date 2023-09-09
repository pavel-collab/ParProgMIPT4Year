//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>

// global variabl for the synchronisation
omp_lock_t my_lock;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[-] Usage %s n -- the number of threads\n", argv[0]);
        return -1;
    }
    // initialise variabl for the synchronisation
    omp_init_lock(&my_lock);

    uint32_t size = atoi(argv[1]);
    uint32_t rank = 0;
    uint32_t flag = 0; // shared var

    // set the number of threads
    omp_set_num_threads(size);

    #pragma omp parallel private(rank) shared(flag)
    {
        rank = omp_get_thread_num();
        // this feature made to make threads work with shared variable one by one
        while (rank > flag) {}

        // Lock execution for other threads
        omp_set_lock(&my_lock);
        printf("Thread id is [%d], flag = %d\n", rank, flag);
        flag++;
        omp_unset_lock(&my_lock);

        #pragma omp barrier 
    }

    omp_destroy_lock(&my_lock);
    return 0;
}