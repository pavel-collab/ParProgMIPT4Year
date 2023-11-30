#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>

void PrintArray2File(const char* file_name, double* arr, unsigned ISIZE, unsigned JSIZE) {
    printf("[DEBUG] start to dump result in file\n");
    std::ofstream file{file_name};
    if (file.is_open()) {
        for (size_t i = 0; i < ISIZE; ++i) {
            for (size_t j = 0; j < JSIZE; ++j) {
                file << arr[i*ISIZE + j] << " ";
            }   
            file << "\n";
        }
        file << std::endl;
    }
    file.close();
    printf("[DEBUG] end to dump result in file\n");
}

void fill_arr(double* arr, unsigned ISIZE, unsigned JSIZE) {
    for (size_t i = 0; i < ISIZE; ++i) {
        for (size_t j = 0; j < JSIZE; ++j) {
            arr[i*ISIZE + j] = 10*i + j;
        }   
    }
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage %s ISIZE JSIZE\n", argv[0]);
        return -1;
    }

    unsigned ISIZE = atoi(argv[1]);
    unsigned JSIZE = atoi(argv[2]);
    const char* valid_data_file_name = "valid_data.txt";
    const char* time_file_name = "time.txt";

    double* arr = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    fill_arr(arr, ISIZE, JSIZE);

    auto t_start = std::chrono::high_resolution_clock::now();
    for (size_t i=2; i<ISIZE; i++){
        for (size_t j = 0; j < JSIZE-3; j++){
            arr[i*ISIZE + j] = sin(5*arr[(i-2)*ISIZE + (j+3)]);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    #if VALIDATION
    PrintArray2File(valid_data_file_name, arr, ISIZE, JSIZE);
    #endif

    #if TIME
    FILE* fd = fopen(time_file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);
    #endif

    free(arr);
}