//* g++ -march=native Vectorization.cpp

#include <iostream>
#include <random>
#include <algorithm>
#include <iterator>
#include <cstdlib>
#include <vector>
#include <cassert>
#include <x86intrin.h>
#include <chrono>

int GetIdx(int i, int j, int N) {
    return i*N + j;
}

// Функция записывает двумерный массив значений в файл
// data_file_name -- название файла
// u -- указатель на массив значений размером M*K
// M -- количество столбцов в двумерном массиве
// K -- количество строк в двумерном массиве
void PutData2File(const char* data_file_name, std::vector<float>* u, int M, int K) {
    FILE* fd = fopen(data_file_name, "a");
    if (fd) {
        for (int m = 0; m < M; ++m) {
            for (int k = 0; k < K; ++k) {
                int idx = GetIdx(m, k, M);
                fprintf(fd, "%0.2lf ", (*u)[idx]);
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
void GenRandomVector(std::vector<float>* vec) {
    // First create an instance of an engine.
    std::random_device rnd_device;
    // Specify the engine and distribution.
    std::mt19937 mersenne_engine {rnd_device()};  
    // Generates random integers
    std::uniform_int_distribution<int> dist {1, 10};
    
    auto gen = [&dist, &mersenne_engine]() {
                   return dist(mersenne_engine);
               };

    std::vector<int> tmp(vec->size());
    std::generate(begin(tmp), end(tmp), gen);

    for (int i = 0; i < vec->size(); i++) {
        (*vec)[i] = static_cast<float>(tmp[i]);
    }
}

void Transpose(std::vector<float>* matrix, int size) {
    for (int i = 0; i < size-1; ++i) {
        for (int j = i+1; j < size; ++j) {
            std::swap((*matrix)[GetIdx(i, j, 8)], (*matrix)[GetIdx(j, i, 8)]);
        }
    }
}

void VectorizedMatrixMulriplication(std::vector<float> &matrix1, 
                                    std::vector<float> &matrix2,
                                    std::vector<float>* result) {
    assert(sqrt(matrix1.size()) - sqrt(matrix2.size()) < 1e-6);
    assert(sqrt(matrix1.size()) - sqrt(result->size()) < 1e-6);

    int N = static_cast<int>(sqrt(matrix1.size()));
    std::fill(result->begin(), result->end(), 0.0);
    Transpose(&matrix2, N);

    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            for (size_t k = 0; k < N; k+=8) { 
                // загружаем 8 чисел float (по 32 бита) в регистр на 256 бит
                __m256 va = _mm256_loadu_ps(&matrix1[GetIdx(i, k, N)]);
                __m256 vb = _mm256_loadu_ps(&matrix2[GetIdx(j, k, N)]);

                // умножаем 2 регистра по 256 бит, получая таким образом перемножение каждой 32-битной ячейки
                __m256 temp = _mm256_mul_ps(va, vb);

                // извлекаем 128 старших битов в отдельную переменную
                __m128 vhigh1 = _mm256_extractf128_ps(temp, 1);
                // у регистра temp отбраысваем старшие биты
                // таким образом, мы разделили регистр temp на 2 регистра по 128 бит
                __m128 vhigh2 = _mm256_castps256_ps128(temp);

                // складываем значения всех 32-битных ячеек
                __m128 reslt = _mm_add_ps(vhigh1, vhigh2);

                reslt = _mm_hadd_ps(reslt, reslt);
                reslt = _mm_hadd_ps(reslt, reslt);

                // в конце мы получаем 128-битный регистр, где каждое 32-битное значение -- наш искомый ответ

                // кастуем младшие 32 бита в float и копируем результат в переменную res
                float res = _mm_cvtss_f32(reslt);

                (*result)[GetIdx(i, j, N)] = res;
            }
        }
    }
}

void PrintMatrix(const std::vector<float> &matrix, size_t matrix_size) {
    auto sz = sqrt(matrix.size());
    assert(sz - matrix_size < 1e-6);

    for (size_t i = 0; i < matrix_size; ++i) {
        for (size_t j = 0; j < matrix_size; ++j) {
            printf("%0.2lf ", matrix[GetIdx(i, j, matrix_size)]);
        }
        printf("\n");
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

    std::vector<float> vec1(N*N);
    std::vector<float> vec2(N*N);
    std::vector<float> result(N*N);

    GenRandomVector(&vec1);
    GenRandomVector(&vec2);

    // PutData2File(matrix1_file_name, &vec1, 8, 8);
    // PutData2File(matrix2_file_name, &vec2, 8, 8);

    auto t_start = std::chrono::high_resolution_clock::now();
    VectorizedMatrixMulriplication(vec1, vec2, &result);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "vectorized_matrix_multiplication " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";

    // PutData2File(multiplication_result_file_name, &result, 8, 8);
    return 0;
}