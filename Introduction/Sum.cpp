//* Compilation: g++ -fopenmp main.cpp 

#include <iostream>
#include <omp.h>
#include <math.h>

double Sum(int start, int end) {
    double res = 0;
    for (double i = start; i <= end; ++i) {
        res += 1/i;
    }
    return res;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "[-] Usage %s n D \n\tWhere n is a number of threads and D is a limit of sum\n", argv[0]);
        return -1;
    }

    uint32_t threads_num = atoi(argv[1]); 
    int N = atoi(argv[2]);

    // Check if each thread will have data for calculation
    if (int(floor(N/2)) < threads_num) {
        perror("[-] Every process have to sum at least two numbers.\nSo, total number of processes must be at least 2 times less then N.\n");
        return -1;
    }

    double result = 0;
    // set the number of threads
    omp_set_num_threads(threads_num);

    #pragma omp parallel shared(result)
    {
        int rank = omp_get_thread_num();

        int n_range = N / threads_num;
        int mod_size = N % threads_num;

        double rank_res = 0;

        int rank_start, rank_end = 0;

        if (rank < mod_size) {
            rank_start = rank*(n_range + 1) + 1;
            rank_end = rank_start + n_range;
        }
        else {
            rank_start = rank*n_range + 1 + mod_size;
            rank_end = rank_start + n_range - 1;
        }

        rank_res = Sum(rank_start, rank_end);
        printf("Thread [%d] sum space [%d, %d]. Rank res is %lf\n", rank, rank_start, rank_end, rank_res);

        // For this moment this section is a garanty of threads synchronisation
        #pragma omp critical
        {
            result += rank_res;
        }
    }

    std::cout << "Result is " << result << std::endl;

    return 0;
}