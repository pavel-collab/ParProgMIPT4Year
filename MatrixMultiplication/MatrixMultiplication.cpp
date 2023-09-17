#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <omp.h>
#include <chrono>

//TODO: replace int matrix to template T matrix to be able to use float or double multiplication
//TODO: make macroses for various types of schedule
//TODO: make a macros for validaiton
//TODO: make an experiment to compare different types of schedule
//TODO: improve plots
//TODO: make Shtrassen algorithm implementation

int GetIdx(int i, int j, int N) {
    return i*N + j;
}

// Функция записывает двумерный массив значений в файл
// data_file_name -- название файла
// u -- указатель на массив значений размером M*K
// M -- количество столбцов в двумерном массиве
// K -- количество строк в двумерном массиве
void PutData2File(const char* data_file_name, std::vector<int>* u, int M, int K) {
    FILE* fd = fopen(data_file_name, "a");
    if (fd) {
        for (int m = 0; m < M; ++m) {
            for (int k = 0; k < K; ++k) {
                int idx = GetIdx(m, k, M);
                fprintf(fd, "%d ", (*u)[idx]);
            }
            fprintf(fd, "\n");
        }
        fclose(fd);
    }
}

/*
In modern C++ it’s recommended not to use any time-based seeds and std::rand, 
but instead to use random_device to generate a seed.
*/
void GenRandomVector(std::vector<int>* vec) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  
    // Generates random integers
    std::uniform_int_distribution<int> dist {1, 10};
    
    auto gen = [&dist, &mersenne_engine]() {
                   return dist(mersenne_engine);
               };

    std::generate(begin(*vec), end(*vec), gen);
}

void GenEyeMatrix(std::vector<int>* matrix, size_t matrix_size) {
    auto sz = sqrt(matrix->size());
    assert(sz - matrix_size < 1e-6);

    for (size_t i = 0; i < matrix_size; ++i) {
        for (size_t j = 0; j < matrix_size; ++j) {
            if (i == j) {
                (*matrix)[GetIdx(i, j, matrix_size)] = 1;
            } else {
                (*matrix)[GetIdx(i, j, matrix_size)] = 0;
            }
        }
    }
}

//! ATTENTION, in this function you should to pass a matrix size, not vector size
//! for example, if you have a vector with size 16, the matrix size will be 4.
//! In other case, programm will read a trash from memory
void PrintMatrix(const std::vector<int> &matrix, size_t matrix_size) {
    auto sz = sqrt(matrix.size());
    assert(sz - matrix_size < 1e-6);

    for (size_t i = 0; i < matrix_size; ++i) {
        for (size_t j = 0; j < matrix_size; ++j) {
            printf("%2d ", matrix[GetIdx(i, j, matrix_size)]);
        }
        printf("\n");
    }
}

//! ATTENTION matrix result shold be feeled by zeros
void SimpleMatrixMulriplication(const std::vector<int> &matrix1, 
                                const std::vector<int> &matrix2,
                                std::vector<int>* result) {
    assert(sqrt(matrix1.size()) - sqrt(matrix2.size()) < 1e-6);
    assert(sqrt(matrix1.size()) - sqrt(result->size()) < 1e-6);

    int N = static_cast<int>(sqrt(matrix1.size()));
    std::fill(result->begin(), result->end(), 0.0);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; ++k) { 
                (*result)[GetIdx(i, j, N)] += matrix1[GetIdx(i, k, N)] * matrix2[GetIdx(k, j, N)];
            }
        }
    }
}

void CacheFriendlyMatrixMultiplication(const std::vector<int> &matrix1, 
                                       const std::vector<int> &matrix2,
                                       std::vector<int>* result) {
    assert(sqrt(matrix1.size()) - sqrt(matrix2.size()) < 1e-6);
    assert(sqrt(matrix1.size()) - sqrt(result->size()) < 1e-6);

    int N = static_cast<int>(sqrt(matrix1.size()));
    std::fill(result->begin(), result->end(), 0.0);

    for (size_t i = 0; i < N; ++i) {
        for (size_t k = 0; k < N; ++k) {
            for (size_t j = 0; j < N; ++j) { 
                (*result)[GetIdx(i, j, N)] += matrix1[GetIdx(i, k, N)] * matrix2[GetIdx(k, j, N)];
            }
        }
    }
}

void ParallelSimpleMatrixMultiplication(const std::vector<int> &matrix1, 
                                        const std::vector<int> &matrix2,
                                        std::vector<int>* result) {
    assert(sqrt(matrix1.size()) - sqrt(matrix2.size()) < 1e-6);
    assert(sqrt(matrix1.size()) - sqrt(result->size()) < 1e-6);

    int N = static_cast<int>(sqrt(matrix1.size()));
    std::fill(result->begin(), result->end(), 0.0);
    omp_set_num_threads(4);

    #pragma omp parallel for schedule(static) shared(matrix1, matrix2, result)
    for (size_t i = 0; i < N; ++i) {
        // printf("[DEBUG] Thread %d\n", omp_get_thread_num());
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; ++k) { 
                (*result)[GetIdx(i, j, N)] += matrix1[GetIdx(i, k, N)] * matrix2[GetIdx(k, j, N)];
            }
        }
    }
}

void ParallelCacheFriendlyMatrixMultiplication(const std::vector<int> &matrix1, 
                                        const std::vector<int> &matrix2,
                                        std::vector<int>* result) {
    assert(sqrt(matrix1.size()) - sqrt(matrix2.size()) < 1e-6);
    assert(sqrt(matrix1.size()) - sqrt(result->size()) < 1e-6);

    int N = static_cast<int>(sqrt(matrix1.size()));
    std::fill(result->begin(), result->end(), 0.0);
    omp_set_num_threads(4);

    #pragma omp parallel for schedule(static) shared(matrix1, matrix2, result)
    for (size_t i = 0; i < N; ++i) {
        // printf("[DEBUG] Thread %d\n", omp_get_thread_num());
        for (size_t k = 0; k < N; ++k) {
            for (size_t j = 0; j < N; ++j) { 
                (*result)[GetIdx(i, j, N)] += matrix1[GetIdx(i, k, N)] * matrix2[GetIdx(k, j, N)];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[-] Error, Expected more arguments. Use %s *matrix_size*\n", argv[0]);
        return -1;
    }
    int N = atoi(argv[1]);

    const char* matrix1_file_name = "matrix1.dat";
    const char* matrix2_file_name = "matrix2.dat";
    const char* multiplication_result_file_name = "multiplication_result.dat";
    const char* file_name = "time.dat";

    std::vector<int> vec1(N*N);
    std::vector<int> vec2(N*N);
    std::vector<int> result(N*N);

    GenRandomVector(&vec1);
    GenRandomVector(&vec2);

    // PutData2File(matrix1_file_name, &vec1, N, N);
    // PutData2File(matrix2_file_name, &vec2, N, N);

    #if ALGORITHM == 0
    auto t_start = std::chrono::high_resolution_clock::now();
    SimpleMatrixMulriplication(vec1, vec2, &result);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "simple_matrix_multiplication " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    FILE* fd = fopen(file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);
    #endif

    #if ALGORITHM == 1
    auto t_start = std::chrono::high_resolution_clock::now();
    CacheFriendlyMatrixMultiplication(vec1, vec2, &result);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "cache_friendly_matrix_multiplication " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    FILE* fd = fopen(file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);
    #endif

    #if ALGORITHM == 2
    auto t_start = std::chrono::high_resolution_clock::now();
    ParallelSimpleMatrixMultiplication(vec1, vec2, &result);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "parallel_simple_matrix_multiplication " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    FILE* fd = fopen(file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);
    #endif

    #if ALGORITHM == 3
    auto t_start = std::chrono::high_resolution_clock::now();
    ParallelCacheFriendlyMatrixMultiplication(vec1, vec2, &result);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "parallel_cache_friendly_matrix_multiplication " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    FILE* fd = fopen(file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);
    #endif

    // PutData2File(multiplication_result_file_name, &result, N, N);
    return 0;
}