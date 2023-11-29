#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include "mpi.h"

/*

В этом примере мы имеем дело с истинной зависимостью. 
Значение ячейки значала определяется и только потом используется.

Можно распараллелить по внутреннему циклу + поставить в конце барьерную синхронизацию.
Так же можно распараллелить по внешнему циклу на 2 исполнителя. Один будет считать только четные кейсы, а второй только нечетные.
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
    printf("[DEBUG] copy array start\n");
    double* copy_arr = (double*) calloc(ISIZE*JSIZE, sizeof(double));

    //TODO: может есть какая-то функция стандартной библиотеки
    for (int i = 0; i < ISIZE; ++i) {
        for (int j = 0; j < JSIZE; ++j) {
            copy_arr[i*ISIZE + j] = arr[i*ISIZE + j];
        }
    }

    printf("[DEBUG] copy array end\n");
    return copy_arr;
}

void PutData2FileParallel(int rank, int size, const char* data_file_name, double* arr, int ISIZE, int JSIZE) {
    if (rank == 0) {        
        for (size_t i=0; i<ISIZE; i+=2){
            int special_signal = 0;

            printf("rank = %d step = %ld\n", rank, i);
            // записываем строку в файл
            FILE* fd = fopen(data_file_name, "a");
            for (size_t j = 0; j < JSIZE; j++){
                fprintf(fd, "%lf ", arr[i*ISIZE + j]);
            }
            fprintf(fd, "\n");
            fclose(fd);
            MPI_Send(&special_signal, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            
            if (i+2 >= ISIZE)
                break;

            MPI_Recv(&special_signal, 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } else {
        for (size_t i=1; i<ISIZE; i+=2){
            int special_signal = 69;
            MPI_Recv(&special_signal, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            printf("rank = %d step = %ld\n", rank, i);
            // записываем строку в файл
            FILE* fd = fopen(data_file_name, "a");
            for (size_t j = 0; j < JSIZE; j++){
                fprintf(fd, "%lf ", arr[i*ISIZE + j]);
            }
            fprintf(fd, "\n");
            fclose(fd);

            MPI_Send(&special_signal, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            if (i+2 >= ISIZE)
                break;
        }
    }
}

//* mpic++ task1.cpp -o a.out
//* mpiexec -n 2 ./a.out 
int main(int argc, char* argv[]) {
    const char* file_name = "res.txt";

    if (argc < 3) {
        fprintf(stderr, "Usage %s ISIZE JSIZE\n", argv[0]);
        return -1;
    }

    unsigned ISIZE = atoi(argv[1]);
    unsigned JSIZE = atoi(argv[2]);

    double* arr = (double*) calloc(ISIZE*JSIZE, sizeof(double));
    fill_arr(arr, ISIZE, JSIZE);

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

    if (size != 2) {
        perror("[-] Error starting MPI program. Programm was terminated.\n");
        MPI_Abort(MPI_COMM_WORLD, rc);
    }

    double start = MPI_Wtime();
    if (rank == 0) { // четные строки
        for (size_t i=2; i<ISIZE; i+=2){
            for (size_t j = 0; j < JSIZE-3; j++){
                arr[i*ISIZE + j] = sin(5*arr[(i-2)*ISIZE + (j+3)]);
            }
        }
        PutData2FileParallel(rank, size, file_name, arr, ISIZE, JSIZE);
    }  else { // нечетные строки
        for (size_t i=3; i<ISIZE; i+=2){
            for (size_t j = 0; j < JSIZE-3; j++){
                arr[i*ISIZE + j] = sin(5*arr[(i-2)*ISIZE + (j+3)]);
            }
        }
        PutData2FileParallel(rank, size, file_name, arr, ISIZE, JSIZE);
    }
    double end = MPI_Wtime();
    double rank_time = (end - start)*1000;

    if (rank == size-1) {
        const char* time_file_name = "time.txt";
        FILE* time_out_file = fopen(time_file_name, "a");
        if (time_out_file) 
            fprintf(time_out_file, "%lf ", rank_time);
        fclose(time_out_file);
    }

    // The end of parallel part of program
    MPI_Finalize();

    free(arr);
    return 0;
}