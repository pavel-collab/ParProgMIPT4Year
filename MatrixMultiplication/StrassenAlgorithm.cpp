#include <random>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <vector>
#include <cassert>
#include <cmath>
#include <omp.h>
#include <chrono>

int GetIdx(int i, int j, int N) {
    return i*N + j;
}

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

// Guess, that marix scale is divided by 2 in any recursion step
std::vector<int> Strassen(std::vector<int> A, std::vector<int> B) {
    assert(sqrt(A.size()) - sqrt(B.size()) < 1e-6);
    int N = static_cast<int>(sqrt(A.size()));
    int n = N / 2;

    if (N == 1) {
        std::vector<int> C = {A[0] * B[0]};
        return C;
    }

    std::vector<int> a11;
    std::vector<int> a12;
    std::vector<int> a21;
    std::vector<int> a22;

    std::vector<int> b11;
    std::vector<int> b12;
    std::vector<int> b21;
    std::vector<int> b22;

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            a11.emplace_back(A[GetIdx(i, j, N)]);
            a12.emplace_back(A[GetIdx(i, j+n, N)]);
            a21.emplace_back(A[GetIdx(i+n, j, N)]);
            a22.emplace_back(A[GetIdx(i+n, j+n, N)]);

            b11.emplace_back(B[GetIdx(i, j, N)]);
            b12.emplace_back(B[GetIdx(i, j+n, N)]);
            b21.emplace_back(B[GetIdx(i+n, j, N)]);
            b22.emplace_back(B[GetIdx(i+n, j+n, N)]);
        }
    }

    std::vector<int> s1(n*n);
    std::vector<int> s2(n*n);
    std::vector<int> s3(n*n);
    std::vector<int> s4(n*n);
    std::vector<int> s5(n*n);
    std::vector<int> s6(n*n);
    std::vector<int> s7(n*n);
    std::vector<int> s8(n*n);
    std::vector<int> s9(n*n);
    std::vector<int> s10(n*n);

    std::transform (b12.begin(), b12.end(), b22.begin(), s1.begin(), std::minus<int>());
    std::transform (a11.begin(), a11.end(), a12.begin(), s2.begin(), std::plus<int>());
    std::transform (a21.begin(), a21.end(), a22.begin(), s3.begin(), std::plus<int>());
    std::transform (b21.begin(), b21.end(), b11.begin(), s4.begin(), std::minus<int>());
    std::transform (a11.begin(), a11.end(), a22.begin(), s5.begin(), std::plus<int>());
    std::transform (b11.begin(), b11.end(), b22.begin(), s6.begin(), std::plus<int>());
    std::transform (a12.begin(), a12.end(), a22.begin(), s7.begin(), std::minus<int>());
    std::transform (b21.begin(), b21.end(), b22.begin(), s8.begin(), std::plus<int>());
    std::transform (a11.begin(), a11.end(), a21.begin(), s9.begin(), std::minus<int>());
    std::transform (b11.begin(), b11.end(), b12.begin(), s10.begin(), std::plus<int>());

    auto P1 = Strassen(a11, s1);
    auto P2 = Strassen(s2, b22);
    auto P3 = Strassen(s3, b11);
    auto P4 = Strassen(a22, s4);
    auto P5 = Strassen(s5, s6);
    auto P6 = Strassen(s7, s8);
    auto P7 = Strassen(s9, s10);

    std::vector<int> c11(n*n);
    std::vector<int> c12(n*n);
    std::vector<int> c21(n*n);
    std::vector<int> c22(n*n);

    std::transform (P5.begin(), P5.end(), P4.begin(), c11.begin(), std::plus<int>());
    std::transform (c11.begin(), c11.end(), P2.begin(), c11.begin(), std::minus<int>());
    std::transform (c11.begin(), c11.end(), P6.begin(), c11.begin(), std::plus<int>());

    std::transform (P1.begin(), P1.end(), P2.begin(), c12.begin(), std::plus<int>());
    std::transform (P3.begin(), P3.end(), P4.begin(), c21.begin(), std::plus<int>());

    std::transform (P5.begin(), P5.end(), P1.begin(), c22.begin(), std::plus<int>());
    std::transform (c22.begin(), c22.end(), P3.begin(), c22.begin(), std::minus<int>());
    std::transform (c22.begin(), c22.end(), P7.begin(), c22.begin(), std::minus<int>());

    std::vector<int> result(N*N);

    for (int i = 0; i < n; i++) {
        for (int j = 0 ; j < n; j++) {
            result[GetIdx(i, j, N)] = c11[GetIdx(i, j, n)];
            result[GetIdx(i, j+n, N)] = c12[GetIdx(i, j, n)];
            result[GetIdx(i+n, j, N)] = c21[GetIdx(i, j, n)];
            result[GetIdx(i+n, j+n, N)] = c22[GetIdx(i, j, n)];
        }
    }

    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[-] Error, Expected more arguments. Use %s *matrix_size*\n", argv[0]);
        return -1;
    }
    int N = atoi(argv[1]);

    bool isPowerOfTwo = N && !(N & (N - 1));
    if (!isPowerOfTwo) {
        fprintf(stderr, "[-] Error. *matrix_size* must be power of 2, but %d is not\n", N);
        return -1;
    }

    const char* matrix1_file_name = "matrix1.dat";
    const char* matrix2_file_name = "matrix2.dat";
    const char* multiplication_result_file_name = "multiplication_result.dat";
    const char* file_name = "time.dat";

    std::vector<int> vec1(N*N);
    std::vector<int> vec2(N*N);

    GenRandomVector(&vec1);
    GenRandomVector(&vec2);

    // PutData2File(matrix1_file_name, &vec1, N, N);
    // PutData2File(matrix2_file_name, &vec2, N, N);

    auto t_start = std::chrono::high_resolution_clock::now();
    auto res = Strassen(vec1, vec2);
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "strassen " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    FILE* fd = fopen(file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);

    // PutData2File(multiplication_result_file_name, &res, N, N);

    return 0;
}