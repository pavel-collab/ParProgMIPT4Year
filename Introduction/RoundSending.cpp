//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
// #include <unistd.h>

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[-] Usage %s n -- the number of threads\n", argv[0]);
        return -1;
    }

    uint32_t threads_num = atoi(argv[1]);
    uint32_t num = 0; // shared var 

    // set the number of threads
    omp_set_num_threads(threads_num);

    #pragma omp parallel shared(num)
    {
        uint32_t thread_num = omp_get_thread_num();
        
        // The first option is to use critical
        // But there is one disadvantage: the sequanse of threads that will be 
        // execute this part of code is unindentified.
        #pragma omp critical
        {
            std::cout << "Thread number [" << thread_num << "] num value is [" << num << "]" << std::endl;
            num++;
        }
    }

    return 0;
}