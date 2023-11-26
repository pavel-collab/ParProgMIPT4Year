#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "mpi.h"

/*

В этом примере мы имеем дело с истинной зависимостью. 
Значение ячейки значала определяется и только потом используется.

Можно распараллелить по внутреннему циклу + поставить в конце барьерную синхронизацию.

*/

int GetRankStart(int rank, int size, int N) {
    int n_range = N / size;
    int mod_size = N % size;

    int rank_start = 0;
    if (rank < mod_size) {
        rank_start = rank*(n_range + 1);
    } 
    else {
        rank_start = rank*n_range + mod_size;
    }

    return rank_start;
}

int GetRankEnd(int rank, int size, int N) {
    int n_range = N / size;
    int mod_size = N % size;

    int rank_end = 0;
    int rank_start = GetRankStart(rank, size, N);
    if (rank < mod_size) {
        rank_end = rank_start + n_range;
    } 
    else {
        rank_end = rank_start + n_range - 1;
    }

    return rank_end;
}

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
    for (int i = 0; i < ISIZE; ++i) {
        for (int j = 0; j < JSIZE; ++j) {
            copy_arr[i*ISIZE + j] = arr[i*ISIZE + j];
        }
    }
    return copy_arr;
}

//* mpic++ task1.cpp -o a.out
//! кроме того, можно распараллелить на 2 исполнителя по внешнему циклу (конкретно в этой задаче)
int main(int argc, char* argv[]) {

    unsigned ISIZE = 10;
    unsigned JSIZE = 10;

    // define variables:
    //  rank -- the number of current process
    //  size -- total amount of process
    int rank = 0;
    int size = 0;

    // Initilize parallel part of program
    int rc = MPI_Init(&argc, &argv);
    if (rc != MPI_SUCCESS) {
        perror("[-] Error starting MPI program. Programm was terminated.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    // Get the total number of pocess (save in variable size)
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    // Get the number of current pocess (save in variable rank)
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    auto rank_start = GetRankStart(rank, size, JSIZE-3);
    auto rank_end = GetRankEnd(rank, size, JSIZE-3);

    for (int i = 2; i < ISIZE; ++i) {
        // производим прием новой порции данных
        for (j = rank_start, j <= rank_end; ++j) {
            // вычисления
        }
        // производим отправку новой порции данных
    }

    // The end of parallel part of program
    MPI_Finalize();

    return 0;
}