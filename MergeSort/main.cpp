#include <iostream>
#include <fstream>
#include <cstring>
#include <sstream>
#include <chrono>
#include <omp.h>

//! NOTE: recursion_depth is a recursion depth parametr
//! if it will be too small and the amount of elements will be too large, it can lead to system error
//! operation system will not be able to allocate enoth resources for new threads, cause in this task we use
//! nested parallelism

// #define DEBUG

// Разделение по схеме Lomuto
int partition(int a[], int start, int end)
{
    // Выбираем крайний правый элемент в качестве опорного элемента массива
    int pivot = a[end];

    // элементы, меньшие точки поворота, будут перемещены влево от `pIndex`
    // элементы больше, чем точка поворота, будут сдвинуты вправо от `pIndex`
    // равные элементы могут идти в любом направлении
    int pIndex = start;

    // каждый раз, когда мы находим элемент, меньший или равный опорному, `pIndex`
    // увеличивается, и этот элемент будет помещен перед опорной точкой.
    for (int i = start; i < end; i++)
    {
        if (a[i] <= pivot)
        {
            std::swap(a[i], a[pIndex]);
            pIndex++;
        }
    }

    // поменять местами `pIndex` с пивотом
    std::swap(a[pIndex], a[end]);

    // вернуть `pIndex` (индекс опорного элемента)
    return pIndex;
}

// Процедура быстрой сортировки
void quicksort(int a[], int start, int end)
{
    // базовое условие
    if (start >= end) {
        return;
    }

    // переставить элементы по оси
    int pivot = partition(a, start, end);

    // повторяем подмассив, содержащий элементы, меньшие опорной точки
    quicksort(a, start, pivot - 1);

    // повторяем подмассив, содержащий элементы, превышающие точку опоры
    quicksort(a, pivot + 1, end);
}

void bubbleSort(int* arr, int n1, int n2)
{
    int i, j;
    for (i = n1; i < n2; i++)
        // Last i elements are already
        // in place
        for (j = n1; j < n1 + n2 - i; j++)
            if (arr[j] > arr[j + 1])
                std::swap(arr[j], arr[j + 1]);
}

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
    #ifdef DEBUG
    printf("Thread [%d] execute Bond(n1=%d, m=%d, n2=%d)\n", omp_get_thread_num(), n1, m, n2);
    #endif

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

void ParallelMerge(int* arr, unsigned n1, unsigned n2, int recursion_depth) {
    #ifdef DEBUG
    printf("Thread [%d] execute Merge(n1=%d, n2=%d)\n", omp_get_thread_num(), n1, n2);
    #endif

    unsigned m = (n1 + n2) / 2;
    if (n2 - n1 <= recursion_depth) {
        //! Here we can define the sort algorithm for the limit size of recursion depth

        #if MERGE
        Merge(arr, n1, n2);
        #endif

        #if QUICK
        quicksort(arr, n1, n2);
        #endif

        #if BUBBLE
        bubbleSort(arr, n1, n2);
        #endif 

        return;
    }
    
    #pragma omp task shared(arr)
    {
        ParallelMerge(arr, n1, m, recursion_depth);
    }

    #pragma omp task shared(arr)
    {
        ParallelMerge(arr, m+1, n2, recursion_depth);
    }

    #pragma omp taskwait
    
    Bond(arr, n1, m, n2);
}

int main(int argc, char* argv[]) {
    
    if (argc < 3) {
        fprintf(stderr, "Usage %s file_name n_proc\n", argv[0]);
        return 1;
    }

    int recursion_depth = 10;
    if (argc == 4) {
        recursion_depth = atoi(argv[3]);
    }

    int N_proc = atoi(argv[2]);
    const char* file_name = argv[1];
    int N = 0;
    int* arr = GetArrayFromFile(file_name, &N);
    const char* time_file_name = "time.dat";

    #if MERGE
    printf("Use merge\n");
    #endif

    #if QUICK
    printf("Use quick\n");
    #endif

    #if BUBBLE
    printf("Use bubble\n");
    #endif 
    
    // As our sort function is recursive, it need to enable nested parallelism
    omp_set_nested(1);

    // set the number of threads
    omp_set_num_threads(N_proc);

    #if TIME
    auto t_start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel
    {
        #pragma omp single
        {
            ParallelMerge(arr, 0, N-1, recursion_depth);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();
    std::cout << "N = " << N << " time: " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms\n";
    FILE* fd = fopen(time_file_name, "a");
    fprintf(fd, "%lf ", std::chrono::duration<double, std::milli>(t_end-t_start).count());
    fclose(fd);
    #endif

    #if VALIDTION
    ParallelMerge(arr, 0, N-1, recursion_depth);
    const char* result_file_name = "result.txt";
    PrintArray2File(result_file_name, arr, N);
    #endif

	return 0;
}
