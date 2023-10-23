#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <complex>

#define pi 3.1415

double* GetArrayFromFile(const char* file_name, uint64_t* size) {
    std::string s;
    int n = 0;
    std::fstream file(file_name);
    if (!file.is_open()) {
        printf("[-] File open error\n");
        return nullptr;
    }

    getline(file, s);
    n = atoi(s.c_str());
    *size = n;

    getline(file, s);
    file.close();

    double* arr = (double*) malloc(n * sizeof(double));
    int idx = 0;

    const int N = 256;          // Максимальная длина строки
    char word[N] = {};          // Буфер для считывания строки

    std::stringstream x;        // Создание потоковой переменной
    x << s;                     // Перенос строки в поток
    while (x >> word) {
        arr[idx] = atof(word);
        idx++;
    }                           // выборка слов

    return arr;
}

/**
 * As FFT use complex numbers and complex exponent, we need to represent
 * real signal values as complex values.
*/
std::complex<double>* Cast2Complex(double* arr, uint64_t N) {
    std::complex<double>* complex_arr = (std::complex<double>*) calloc(N, sizeof(std::complex<double>));
    for (int i = 0; i < N; i++) {
        complex_arr[i] = {arr[i], 0};
    }
    return complex_arr;
}

void PrintArray2File(const char* file_name, double* arr, unsigned N) {
    std::ofstream file{file_name};
    if (file.is_open()) {
        file << N << "\n";
        for (size_t i = 0; i < N; ++i) {
            file << floor(arr[i]*10000) / 10000 << " ";
        }
        file << std::endl;
    }
    file.close();
}

/**
 * Function take a complex object array and return the array of real part
 * of each given array member.
*/
double* DescribeRealPart(std::complex<double>* arr, uint64_t N) {
    double* real_arr = (double*) calloc(N, sizeof(double));
    for (int i = 0; i < N; i++) {
        real_arr[i] = arr[i].real();
    }
    return real_arr;
}

/**
 * Function take a complex object array and return the array of imag part
 * of each given array member.
*/
double* DescribeImagPart(std::complex<double>* arr, uint64_t N) {
    double* imag_arr = (double*) calloc(N, sizeof(double));
    for (int i = 0; i < N; i++) {
        imag_arr[i] = arr[i].imag();
    }
    return imag_arr;
}

/**
 * Naive implementation of Fourier transform
*/
std::complex<double>* SlowFT(std::complex<double>* signal, uint64_t N) {
    const std::complex<double> i(0.0, 1.0);
    std::complex<double>* spectr = (std::complex<double>*) calloc(N, sizeof(std::complex<double>));

    for (int k = 0; k < N; ++k) {
        for (int n = 0; n < N; ++n) {
            spectr[k] += signal[n] * std::exp(- 2* pi * k * n / N * i);
        }
    }

    return spectr;
}

// void SplitSignalHonest(std::complex<double>* signal, std::complex<double>* honest_part, uint64_t N) {
//     size_t honest_idx = 0;
//     for (size_t i = 0; i < N; ++i) {
//         if (i  % 2 ==0) {
//             honest_part[honest_idx] = signal[i];
//             honest_idx++;
//         }
//     }
// }

// void SplitSignalOdd(std::complex<double>* signal, std::complex<double>* negative_part, uint64_t N) {
//     size_t negative_idx = 0;
//     for (size_t i = 0; i < N; ++i) {
//         if (i  % 2 != 0) {
//             negative_part[negative_idx] = signal[i];
//             negative_idx++;
//         }
//     }
// }

/**
 * With fast Fourier transform algorithm we split given array of signals to parts with honest
 * and odd indexes. This function produce two arrays with honest and odd indexes elements.
*/
void SplitSignal2HonestAndOdd(std::complex<double>* signal,
                              std::complex<double>* honest_part,
                              std::complex<double>* odd_part, 
                              uint64_t N) {
    for (size_t i = 0; i < N/2; i++) {
        honest_part[i] = signal[2*i];
        odd_part[i] = signal[2*i + 1];
    }
}

/**
 * As FFT algorithm is recursive, when we contain two array parts, we should to use it
 * to produce final spectr values. 
*/
std::complex<double>* ConcatenatePositiveAndNegative(std::complex<double>* positive_part, std::complex<double>* negative_part, uint64_t n) {
    std::complex<double>* result = (std::complex<double>*) calloc(n*2, sizeof(std::complex<double>));

    const std::complex<double> i(0.0, 1.0);
    
    std::complex<double> factor_1;
    std::complex<double> factor_2;

    for (size_t k = 0; k < n; ++k) {
        factor_1 = std::exp(-2*pi*k / (2*n) * i);;
        factor_2 = std::exp(-2*pi*(k + n)/ (2*n) * i);

        result[k] = positive_part[k] + negative_part[k] * factor_1;
        result[k + n] = positive_part[k] + negative_part[k] * factor_2;
    }

    return result;
}

std::complex<double>* SlowFFT(std::complex<double>* signal, uint64_t N) {
    std::complex<double>* spectr = (std::complex<double>*) calloc(N, sizeof(std::complex<double>));

    // When we achive the depth of recursion, we just use naive algorithm.
    if (N < 32) {
        return SlowFT(signal, N);
    } else {
        std::complex<double>* x_even = (std::complex<double>*) calloc(N/2, sizeof(std::complex<double>));
        std::complex<double>* x_odd = (std::complex<double>*) calloc(N/2, sizeof(std::complex<double>));

        // SplitSignalHonest(signal, x_even, N);
        // SplitSignalOdd(signal, x_odd, N);

        SplitSignal2HonestAndOdd(signal, x_even, x_odd, N);

        auto fft_even = SlowFFT(x_even, N/2);
        auto fft_odd = SlowFFT(x_odd, N/2);
        auto res = ConcatenatePositiveAndNegative(fft_even, fft_odd, N/2);

        free(fft_even);
        free(fft_odd);
        free(x_even);
        free(x_odd);
        return res;
    }

    return spectr;
}

int main() {
    const char* signal_file_path = "./data/custom_signal.dat";
    
    const char* real_result_file_path = "./fft_real.dat";
    const char* imag_result_file_path = "./fft_imag.dat";

    // The number of values in signal array
    uint64_t N;

    // Firstly we import the array with signal values in time from the file
    // Signal values are generated by python script
    double* signal = GetArrayFromFile(signal_file_path, &N);

    /*
    * As FFT use complex numbers and complex exponent, we need to represent
    * real signal values to complex values. Functions that calculates FFT work with complex value arrays.
    */
    auto complex_signal = Cast2Complex(signal, N);

    // Here we calculate FFT of signal and contain the result in complex value array
    auto spectr = SlowFFT(complex_signal, N);

    // As complex objects are difficult to analyze, we should to represent it through real and imaginary parts
    auto spectr_real = DescribeRealPart(spectr, N);
    auto spectr_imag = DescribeImagPart(spectr, N);

    // Now we prit the result to the files to use, analyze and validate it
    PrintArray2File(real_result_file_path, spectr_real, N);
    PrintArray2File(imag_result_file_path, spectr_imag, N);

    free(spectr_real);
    free(spectr_imag);

    free(complex_signal);
    free(signal);
    free(spectr);
    return 0;
}