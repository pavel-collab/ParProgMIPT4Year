#include <iostream>
#include <cmath>
#include <chrono>
#include <omp.h>

double Task1(double* a, uint64_t ISIZE, uint64_t JSIZE) {
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            a[i*ISIZE + j] = 10*i +j;
        }
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 3; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE-2; j++){
            a[i*ISIZE + j] = sin(5*a[(i-2) * ISIZE + j + 3]);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    std::cout << "Task1: t = " << time << " ms\n";

    FILE *ff;
    ff = fopen("./check_results/check_result_task1.txt", "w");
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            fprintf(ff, "%f ", a[i*ISIZE + j]);
        }
        fprintf(ff,"\n");
    }

    fclose(ff);
    return time;
}

double Task2(double* a, uint64_t ISIZE, uint64_t JSIZE) {
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            a[i*ISIZE + j] = 10*i +j;
        }
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ISIZE-1; i++){
        for (uint64_t j = 6; j < JSIZE; j++){
            a[i*ISIZE + j] = sin(0.2*a[(i+1)*ISIZE + j-6]);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    std::cout << "Task2: t = " << time << " ms\n";

    FILE *ff;
    ff = fopen("./check_results/check_result_task2.txt", "w");
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            fprintf(ff, "%f ", a[i*ISIZE + j]);
        }
        fprintf(ff,"\n");
    }

    fclose(ff);
    return time;
}

double Task3(double* a, double* b, uint64_t ISIZE, uint64_t JSIZE) {
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            a[i*ISIZE + j] = 10*i +j;
            b[i*ISIZE + j] = 0;
        }
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            a[i*ISIZE + j] = sin(0.01*a[i*ISIZE + j]);
        }
    }

    for (uint64_t i = 0; i < ISIZE-1; i++){
        for (uint64_t j = 3; j < JSIZE; j++){
            b[i*ISIZE + j] = a[(i+1)*ISIZE + j-3]*2;
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    double time = std::chrono::duration<double, std::milli>(t_end-t_start).count();
    std::cout << "Task3: t = " << time << " ms\n";

    FILE *ff;
    ff = fopen("./check_results/check_result_task3.txt", "w");
    for (uint64_t i = 0; i < ISIZE; i++){
        for (uint64_t j = 0; j < JSIZE; j++){
            fprintf(ff, "%f ", b[i*ISIZE + j]);
        }
        fprintf(ff,"\n");
    }

    fclose(ff);
    return time;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "[-] Error. Expected more arguments.\n");
        fprintf(stderr, "Usage %s ISIZE JSIZE\n", argv[0]);
        return -1;
    }

    uint64_t ISIZE = atoll(argv[1]);
    uint64_t JSIZE = atoll(argv[2]);

    double* a = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    double* b = (double*) calloc(ISIZE*JSIZE, sizeof(double));

    Task1(a, ISIZE, JSIZE);
    Task2(a, ISIZE, JSIZE);
    Task3(a, b, ISIZE, JSIZE);

    return 0;
}