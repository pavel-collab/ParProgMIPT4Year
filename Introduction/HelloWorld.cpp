//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
#include <unistd.h>

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
        uint32_t thread_num = omp_get_thread_num();
        // if we don't do it, lot of threads access to the stdout at the same time,
        // and we have race condition with output resource.
        // If you're really interested in it, you can just comment this part of code,
        // and try to compile and run the program
        sleep(thread_num);

        std::cout << "Hello, world! Thread number [" << thread_num << "]" << std::endl;
        // barrier synchronisation
        #pragma omp barrier 

        // We also can use #pragma omp critical, but it this case
        // the sequanse of threads is unindentified
        // #pragma omp critical
        // {
        //     std::cout << "Hello, world! Thread number [" << thread_num << "]" << std::endl;
        // }
    }

    return 0;
}