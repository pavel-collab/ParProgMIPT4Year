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
    double* copy_arr = (double*) calloc(ISIZE*JSIZE, sizeof(double));

    //TODO: может есть какая-то функция стандартной библиотеки
    #pragma omp parallel for shared(copy_arr, arr)
    for (int i = 0; i < ISIZE; ++i) {
        for (int j = 0; j < JSIZE; ++j) {
            copy_arr[i*ISIZE + j] = arr[i*ISIZE + j];
        }
    }
    return copy_arr;
}

double seq_impl(double* arr, unsigned ISIZE, unsigned JSIZE) {
    // Start to measure work time
    auto t_start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ISIZE-1; ++i) {
        for (size_t j = 6; j < JSIZE; ++j) {
            arr[i*ISIZE + j] = sin(0.2*arr[(i+1)*ISIZE + (j-6)]);
        }   
    }

    // End to measure work time
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "[seq_impl] " << "ISIZE = " << ISIZE << " JSIZE = " << JSIZE << " time: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    return std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

double par_impl_both(double* arr, unsigned ISIZE, unsigned JSIZE) {
    // Start to measure work time
    auto t_start = std::chrono::high_resolution_clock::now();

    auto copy_arr = copy_array(arr, ISIZE, JSIZE);

    #pragma omp parallel for private(copy_arr)
    for (size_t i = 0; i < ISIZE-1; ++i) {
        #pragma omp barrier
        #pragma omp parallel for
        for (size_t j = 6; j < JSIZE; ++j) {
            arr[i*ISIZE + j] = sin(0.2*copy_arr[(i+1)*ISIZE + (j-6)]);
        }   
    }

    // End to measure work time
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "[par_impl_both] " << "ISIZE = " << ISIZE << " JSIZE = " << JSIZE << " time: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    
    free(copy_arr);
    return std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

double par_impl_outter(double* arr, unsigned ISIZE, unsigned JSIZE) {
    // Start to measure work time
    auto t_start = std::chrono::high_resolution_clock::now();

    auto copy_arr = copy_array(arr, ISIZE, JSIZE);

    #pragma omp parallel for private(copy_arr)
    for (size_t i = 0; i < ISIZE-1; ++i) {
        for (size_t j = 6; j < JSIZE; ++j) {
            arr[i*ISIZE + j] = sin(0.2*copy_arr[(i+1)*ISIZE + (j-6)]);
        }   
    }

    // End to measure work time
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "[par_impl_outter] " << "ISIZE = " << ISIZE << " JSIZE = " << JSIZE << " time: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
   
    free(copy_arr);
    return std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

double par_impl_inner(double* arr, unsigned ISIZE, unsigned JSIZE) {
    // Start to measure work time
    auto t_start = std::chrono::high_resolution_clock::now();

    for (size_t i = 0; i < ISIZE-1; ++i) {
        #pragma omp barrier
        #pragma omp parallel for
        for (size_t j = 6; j < JSIZE; ++j) {
            arr[i*ISIZE + j] = sin(0.2*arr[(i+1)*ISIZE + (j-6)]);
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

    double* arr = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    fill_arr(arr, ISIZE, JSIZE);

    auto seq_time = seq_impl(arr, ISIZE, JSIZE);
    PrintArray2File(valid_data_file_name, arr, ISIZE, JSIZE);

    // auto par_inner_time = par_impl_inner(arr, ISIZE, JSIZE);
    // PrintArray2File(file_name, arr, ISIZE, JSIZE);

    // auto par_outter_time = par_impl_outter(arr, ISIZE, JSIZE);
    // PrintArray2File(file_name, arr, ISIZE, JSIZE);

    // auto par_both_time = par_impl_both(arr, ISIZE, JSIZE);
    // PrintArray2File(file_name, arr, ISIZE, JSIZE);

    free(arr);
}

/*

В данном примере значение массива сначала используется, потом перезаписывается. 
Нарушение 3го условия Бернстрайна. Анизависимость.

В рамках внутреннего цикла конфликтов не будет. 
Конфликты могут возникнуть между итерациями внешнего цикла.

Если при входе во внешний цикл мы продублируем массив, то конфликтов не будет.

*/