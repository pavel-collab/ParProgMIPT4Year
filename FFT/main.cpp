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

    const int N = 256;      //Максимальная длина строки
    char word[N] = {};          //Буфер для считывания строки

    std::stringstream x;        //Создание потоковой переменной
    x << s;                //Перенос строки в поток
    while (x >> word) {
        arr[idx] = atof(word);
        idx++;
    } //выборка слов

    return arr;
}

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

double* DescribeRealPart(std::complex<double>* arr, uint64_t N) {
    double* real_arr = (double*) calloc(N, sizeof(double));
    for (int i = 0; i < N; i++) {
        real_arr[i] = arr[i].real();
    }
    return real_arr;
}

double* DescribeImagPart(std::complex<double>* arr, uint64_t N) {
    double* imag_arr = (double*) calloc(N, sizeof(double));
    for (int i = 0; i < N; i++) {
        imag_arr[i] = arr[i].imag();
    }
    return imag_arr;
}

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

void SpltSigralPositive(std::complex<double>* signal, std::complex<double>* positive_part, uint64_t N) {
    size_t positive_idx = 0;
    for (size_t i = 0; i < N; ++i) {
        if (i  % 2 ==0) {
            positive_part[positive_idx] = signal[i];
            positive_idx++;
        }
    }
}

void SpltSigralNegative(std::complex<double>* signal, std::complex<double>* negative_part, uint64_t N) {
    size_t negative_idx = 0;
    for (size_t i = 0; i < N; ++i) {
        if (i  % 2 != 0) {
            negative_part[negative_idx] = signal[i];
            negative_idx++;
        }
    }
}

std::complex<double>* ConcatenatePositiveAndNegative(std::complex<double>* positive_part, std::complex<double>* negative_part, uint64_t n) {
    std::complex<double>* result = (std::complex<double>*) calloc(n*2, sizeof(std::complex<double>));

    const std::complex<double> i(0.0, 1.0);
    std::complex<double> factor;
    size_t idx = 0;

    for (size_t k = 0; k < n; ++k) {
        factor = std::exp(-2*pi*k / (2*n) * i);
        result[idx] = positive_part[k] + negative_part[k] * factor;
        idx++;
    }
    for (size_t k = 0; k < n; ++k) {
        factor = std::exp(-2*pi*(k + n)/ (2*n) * i);
        result[idx] = positive_part[k] + negative_part[k] * factor;
        idx++;
    }
    return result;
}

std::complex<double>* SlowFFT(std::complex<double>* signal, uint64_t N) {
    std::complex<double>* spectr = (std::complex<double>*) calloc(N, sizeof(std::complex<double>));

    if (N < 32) {
        return SlowFT(signal, N);
    } else {
        std::complex<double>* x_even = (std::complex<double>*) calloc(N/2, sizeof(std::complex<double>));
        std::complex<double>* x_odd = (std::complex<double>*) calloc(N/2, sizeof(std::complex<double>));

        SpltSigralPositive(signal, x_even, N);
        SpltSigralNegative(signal, x_odd, N);

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
    uint64_t N;

    double* signal = GetArrayFromFile(signal_file_path, &N);
    auto complex_signal = Cast2Complex(signal, N);

    const char* result_file_path = "./fft.dat";
    // auto spectr = SlowFT(complex_signal, N);
    auto spectr = SlowFFT(complex_signal, N);

    auto spectr_real = DescribeRealPart(spectr, N);
    PrintArray2File(result_file_path, spectr_real, N);

    free(spectr_real);
    free(complex_signal);
    free(signal);
    free(spectr);
    return 0;
}