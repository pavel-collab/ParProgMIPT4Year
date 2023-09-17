#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <omp.h>

int GetIdx(int i, int j, int N) {
    return i*N + j;
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
    std::uniform_int_distribution<int> dist {1, 52};
    
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

//TODO: use the std::fill
void GenZeroMatrix(std::vector<int>* matrix) {
    for (size_t i = 0; i < matrix->size(); ++i) {
        (*matrix)[i] = 0;
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

void ParallelSimpleMatrixMultiplication(const std::vector<int> &matrix1, 
                                        const std::vector<int> &matrix2,
                                        std::vector<int>* result) {
    assert(sqrt(matrix1.size()) - sqrt(matrix2.size()) < 1e-6);
    assert(sqrt(matrix1.size()) - sqrt(result->size()) < 1e-6);

    int N = static_cast<int>(sqrt(matrix1.size()));

    #pragma omp parallel for schedule(static) shared(matrix1, matrix2, result)
    for (size_t i = 0; i < N; ++i) {
        printf("[DEBUG] Thread %d\n", omp_get_thread_num());
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; ++k) { 
                (*result)[GetIdx(i, j, N)] += matrix1[GetIdx(i, k, N)] * matrix2[GetIdx(k, j, N)];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    omp_set_dynamic(true);

    int N = 2;
    std::vector<int> v1(4);
    GenEyeMatrix(&v1, N);
    const std::vector<int> v2 = {1, 2, 3, 4};

    std::vector<int> result(4, 0);

    ParallelSimpleMatrixMultiplication(v1, v2, &result);
    // CacheFriendlyMatrixMultiplication(v1, v2, &result);
    PrintMatrix(result, N);
    printf("\n");

    GenZeroMatrix(&result);

    ParallelSimpleMatrixMultiplication(v2, v2, &result);
    // CacheFriendlyMatrixMultiplication(v2, v2, &result);
    PrintMatrix(result, N);
}