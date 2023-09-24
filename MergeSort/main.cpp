#include <iostream>
#include <fstream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <chrono>

//! не забываем освобождать массив в конце программы
int* GetArrayFromFile(const char* file_name, int* size) {
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

    int* arr = (int*) calloc(n, sizeof(int));
    int idx = 0;

    const int N = 256;      //Максимальная длина строки
    char word[N] = {};          //Буфер для считывания строки

    std::stringstream x;        //Создание потоковой переменной
    x << s;                //Перенос строки в поток
    while (x >> word) {
        arr[idx] = atoi(word);
        idx++;
    } //выборка слов

    return arr;
}

void PrintArray(int* arr, unsigned N) {
    for (size_t i = 0; i < N; ++i) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}

void PrintArray2File(const char* file_name, int* arr, unsigned N) {
    std::ofstream file{file_name};
    if (file.is_open()) {
        file << N << "\n";
        for (size_t i = 0; i < N; ++i) {
            file << arr[i] << " ";
        }
        file << std::endl;
    }
    file.close();
}

//! следим за тем, чтобы функция копирования не выходила за границу массива 
void CopyArray(int* src, int* dst, unsigned a, unsigned b) {
    unsigned dst_itr = 0;
    for (unsigned i = a; i <= b; ++i) {
        dst[dst_itr] = src[i];
        dst_itr++;
    }
}

void Bond(int* arr, unsigned n1, unsigned m, unsigned n2) {
    unsigned N1 = m - n1 + 1;
    unsigned N2 = n2 - m;

    int* sub_arr1 = (int*) malloc(N1*sizeof(int));
    int* sub_arr2 = (int*) malloc(N2*sizeof(int));
    CopyArray(arr, sub_arr1, n1, m);
    CopyArray(arr, sub_arr2, m+1, n2);

    unsigned itr1 = 0;
    unsigned itr2 = 0;
    unsigned main_itr = n1;

    while ((itr1 < N1) && (itr2 < N2)) {
        if (sub_arr1[itr1] < sub_arr2[itr2]) {
            arr[main_itr] = sub_arr1[itr1];
            itr1++;
        }
        else {
            arr[main_itr] = sub_arr2[itr2];
            itr2++;
        }
        main_itr++;
    }

    if (itr1 == N1) {
        while(itr2 < N2) {
            arr[main_itr] = sub_arr2[itr2];
            itr2++;
            main_itr++;
        }
    }
    else {
        while(itr1 < N1) {
            arr[main_itr] = sub_arr1[itr1];
            itr1++;
            main_itr++;
        }
    }
    free(sub_arr1);
    free(sub_arr2);
}

// Функция сортирует массив arr от элемента с индексом n1 до элемента с индексом n2
void Merge(int* arr, unsigned n1, unsigned n2) {
    
    unsigned m = (n1 + n2) / 2;
    if (n1 == n2)
        return;
    else {
        Merge(arr, n1, m);
        Merge(arr, m+1, n2);
        Bond(arr, n1, m, n2);
    }
}

int main(int argc, char* argv[]) {
	return 0;
}
