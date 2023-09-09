//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
#include <math.h>
#include <cstring>

int main(int argc, char* argv[]) {

    // This function controls the amount of available threads for correct enviroment working
    omp_set_dynamic(true);

    // This funciton enables nested parallelism
    omp_set_nested(1);
    // Check if we enabled nested parallel
    if (!omp_get_nested()) {
        perror("[-] Error with enabled nested parallel.\n");
    }

    #pragma omp parallel
    {
        /*
        Note: if thread in parallel section find one more parallel section, it creates a new 
        group of threads and becomes the main thread for this group.

        Inside the nested parallel section new independet thread numeration exists.
        */
        uint32_t rank = omp_get_thread_num();
        uint32_t size = omp_get_num_threads();
        printf("Thread %d/%d -- (nested level %d)\n", rank, size, omp_get_level());
        #pragma omp parallel
        {
            /*
            If we use omp_get_thread_num() or omp_get_num_threads() inside nested section
            it will return independent values that don't correlate with the parent parallel section.
            */
            uint32_t nested_rank = omp_get_thread_num();
            uint32_t nested_size = omp_get_num_threads();
            printf("\tThread %d.%d/%d -- (nested level %d)\n", rank, nested_rank, nested_size, omp_get_level());

            #pragma omp parallel
            {
                uint32_t nn_rank = omp_get_thread_num();
                uint32_t nn_size = omp_get_num_threads();
                printf("\t\tThread %d.%d.%d/%d -- (nested level %d)\n", rank, nested_rank, nn_rank, nn_size, omp_get_level());
            }
        }
        #pragma omp barrier 
    }

    return 0;
}