//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
#include <math.h>
#include <cstring>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "[-] Usage %s n D \n\tWhere n is a number of threads and D is a limit of sum\n", argv[0]);
        return -1;
    }

    uint32_t size = atoi(argv[1]); 
    int N = atoi(argv[2]);

    // Check if each thread will have data for calculation
    if (int(floor(N/2)) < size) {
        perror("[-] Every process have to sum at least two numbers.\nSo, total number of processes must be at least 2 times less then N.\n");
        return -1;
    }

    double result = 0;
    // set the number of threads
    omp_set_num_threads(size);

    /*
    There are 3 different types of balancing work between threads.
    You can set the type of balancing by the comand line parametr.
    The default type, that used, if you didn't set any type by yourself is static.
    */
    if ((argc == 4 && !strcmp(argv[3], "static")) || argc == 3) {
        #pragma omp parallel reduction(+: result)
        {        
            /*
            With static type each thread will take equal amount of iterations.
            That means, for example, if there are 1000 iterations in cycle, the first
            thread will execute iterations from 1 to 250, the second from 251 to 500 etc.
            */
            double local_res = 0; // this variable need to view what part of harmonic row this proces has sum
            
            #pragma omp for schedule(static)
            for (int i = 1; i <= N; ++i) {
                local_res += 1/static_cast<double>(i);
                result += 1/static_cast<double>(i);
            }

            int rank = omp_get_thread_num();
            printf("Thread [%d]. Rank res is %lf\n", rank, local_res);
        }
    } else if (argc == 4 && !strcmp(argv[3], "dynamic")) {
        #pragma omp parallel reduction(+: result)
        {        
            /*
            Each thread takes a package of iterations fixed size. When some thread
            finish it's chunk of the work, it will take next package.
            */
            double local_res = 0; // this variable need to view what part of harmonic row this proces has sum
            
            #pragma omp for schedule(dynamic)
            for (int i = 1; i <= N; ++i) {
                local_res += 1/static_cast<double>(i);
                result += 1/static_cast<double>(i);
            }

            int rank = omp_get_thread_num();
            printf("Thread [%d]. Rank res is %lf\n", rank, local_res);
        }
    } else if (argc == 4 && !strcmp(argv[3], "guided")) {
        #pragma omp parallel reduction(+: result)
        {        
            /*
            This type of balancing works the same with type "dynamic", but the size of
            data package could change depends of unexecuted iteraions.
            */
            double local_res = 0; // this variable need to view what part of harmonic row this proces has sum
            
            #pragma omp for schedule(guided)
            for (int i = 1; i <= N; ++i) {
                local_res += 1/static_cast<double>(i);
                result += 1/static_cast<double>(i);
            }

            int rank = omp_get_thread_num();
            printf("Thread [%d]. Rank res is %lf\n", rank, local_res);
        }
    } else {
        fprintf(stderr, "[-] Unexpected type of synchronisation %s\n", argv[3]);
        return -1;
    }

    std::cout << "Result is " << result << std::endl;

    return 0;
}