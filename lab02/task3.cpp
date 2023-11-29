#include <iostream>
#include <omp.h>
#include <chrono>
#include <cmath>
#include <fstream>
#include <sstream>

void PrintArray2File(const char* file_name, double* arr, unsigned ISIZE, unsigned JSIZE) {
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
}

void fill_arr(double* arr, unsigned ISIZE, unsigned JSIZE) {
    for (size_t i = 0; i < ISIZE; ++i) {
        for (size_t j = 0; j < JSIZE; ++j) {
            arr[i*ISIZE + j] = 10*i + j;
        }   
    }
}

double* copy_array(const double* arr, unsigned ISIZE, unsigned JSIZE) {
    printf("[DEBUG] copy array start\n");
    double* copy_arr = (double*) calloc(ISIZE*JSIZE, sizeof(double));

    //TODO: может есть какая-то функция стандартной библиотеки
    for (int i = 0; i < ISIZE; ++i) {
        for (int j = 0; j < JSIZE; ++j) {
            copy_arr[i*ISIZE + j] = arr[i*ISIZE + j];
        }
    }

    printf("[DEBUG] copy array end\n");
    return copy_arr;
}

double seq_impl(double* a, double* b, unsigned ISIZE, unsigned JSIZE) {
    // Start to measure work time
    auto t_start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ISIZE; ++i) {
        for (size_t j = 0; j < JSIZE; ++j) {
            a[i*ISIZE + j] = sin(0.01*a[i*ISIZE + j]);

            if ((i >= 1) && (j >= 3)) {
                b[(i-1)*ISIZE + j] = a[i*ISIZE + (j-3)]*2;
            }
        }   
    }

    // End to measure work time
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "[seq_impl] " << "ISIZE = " << ISIZE << " JSIZE = " << JSIZE << " time: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    return std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

double par_impl_inner(double* a, double* b, unsigned ISIZE, unsigned JSIZE) {
    // Start to measure work time
    auto t_start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ISIZE+1; ++i) {
        #pragma omp barrier
        #pragma omp parallel for shared(a, b)
        for (size_t j = 0; j < JSIZE; ++j) {
            if (i < ISIZE)
                a[i*ISIZE + j] = sin(0.01*a[i*ISIZE + j]);
            
            //! сделаем сдвиг циклов
            if ((i >= 2) && (j >= 3)) {
                b[(i-2)*ISIZE + j] = a[(i-1)*ISIZE + (j-3)]*2;
            }
        }   
    }

    // End to measure work time
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "[par_impl_inner] " << "ISIZE = " << ISIZE << " JSIZE = " << JSIZE << " time: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    return std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        fprintf(stderr, "Usage %s ISIZE JSIZE proces_number\n", argv[0]);
        return -1;
    }

    unsigned ISIZE = atoi(argv[1]);
    unsigned JSIZE = atoi(argv[2]);
    size_t proces_number = atoi(argv[3]);

    omp_set_num_threads(proces_number);
    omp_set_nested(1);

    const char* valid_data_file_name = "valid_data.txt";
    const char* file_name = "res.txt";

    double* a = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    double* b = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    fill_arr(a, ISIZE, JSIZE);

    auto seq_time = seq_impl(a, b, ISIZE, JSIZE);
    PrintArray2File(valid_data_file_name, b, ISIZE, JSIZE);
    // auto inner_time = par_impl_inner(a, b, ISIZE, JSIZE);
    // PrintArray2File(file_name, b, ISIZE, JSIZE);

    free(a);
    free(b);
}