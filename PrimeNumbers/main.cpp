#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <algorithm> 
#include <vector>
#include <cstring>
#include <sstream>
#include <fstream>

// Процедура strike вычеркивает составные числа в интервале [start; limit) с шагом step.
// Возвращает значение limit%step==0? limit: limit-step.
void strike(int start, const int limit, const int step, bool* is_composite) {
    for (; start<= limit; start += step)
        is_composite[start] = false;
}

// возвращает вектор простых чисел из первого поддиапазона
std::vector<int> get_first_batcn_prime_numbers(bool* is_composite, const size_t batch_size) {
    // хранит простые числа, найденные в самом первом поддиапазоне.
    // Эти числа ищутся обычным алгоритмом
    std::vector<int> prime_steps;
    
    // Найдем все простые числа в первом поддиапазоне с помощью классического алгоритма
    for (int i = 3; i <= batch_size; i += 2)  {
        // если число простое
        if(is_composite[i/2]) {
            // с шагом равному простому числу перемещаемся по интервалу [простое число/2; m/2]
            // в результате вычеркнем все числа, делящиеся на простое число i (модифицируем массив is_composite)
            strike(i/2+i, batch_size/2, i, is_composite);
            // запомним шаг\новое простое число
            prime_steps.emplace_back(i);
        }
    }

    return prime_steps;   
}

int get_start_strike_position(const int batch_number, const int batch_size, const int prime_number) {
    int start = batch_number * batch_size;
    int p = start % prime_number;

    int l = 0;
    if (!(p&1)) {
        l = start + (prime_number - p);
    } else {
        l = start + (2*prime_number - p);
    }

    int pos = (l-1) / 2;
    return pos;
}

std::vector<std::vector<int>> get_all_start_positions(const std::vector<int>& prime_steps, const int batch_size) {
    std::vector<std::vector<int>> start_positions;
    // для каждого простого числа из первого поддиапазона
    for (auto prime : prime_steps) {
        std::vector<int> starts_for_prime;
        // для каждого батча находим позицию, с которой надо начать вычеркивать числа
        // кратные заданному простому числу
        for (int i = 1; i < batch_size; ++i) {
            auto pos = get_start_strike_position(i, batch_size, prime);
            starts_for_prime.emplace_back(pos);
        }
        start_positions.emplace_back(starts_for_prime);
    }

    return start_positions;
}

void print_prime_numbers_to_file(const char* file_path, bool* is_composite, const int N) {
    std::ofstream file{file_path};
    if (file.is_open()) {
        file << N << "\n";
        for (int i = 0; i < N/2; ++i) {
            if (is_composite[i]) {
                file << 2*i + 1 << " ";
            }
        }
        file << std::endl;
    }
    file.close();
}

double parallel_sieve(const std::vector<int>& prime_steps, const std::vector<std::vector<int>>& start_positions, const int batch_size, bool* is_composite) {
    auto t_start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for schedule(static,1) shared(is_composite)
    // начинаем обходить массив чисел (шагаем целыми батчами) i -- нормер батча
    for (int i = 1; i < batch_size; i++) {
        // для выбранного батча перебираем простые числа, 
        // смотрим позицию, с которой надо начать вычеркавание в этом батче для этого простого числа
        for (int j = 0; j < prime_steps.size(); ++j) {
            auto prime = prime_steps[j];
            auto start = start_positions[j][i-1];
            strike(start, ((i+1)*batch_size - 1) / 2, prime, is_composite);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double, std::milli>(t_end-t_start).count();
}

int main(int argc, char* argv[]) {
    // в параметрах программы задаем N и количество потоков
    if (argc < 3) {
        fprintf(stderr, "Usage %s N proces_number", argv[0]);
        return -1;
    }
    
    const char* res_file_path = "result.txt";
    const char* time_file_path = "time.txt";
    int N = atoi(argv[1]);
    int proces_number = atoi(argv[2]);
    int num_of_odd_number = N/2;

    // вычисляем размер поддиапазона
    int batch_size = sqrt(N);
    batch_size += batch_size&1;

    // set the number of threads
    omp_set_num_threads(proces_number);


    /*
    Поскольку четные числа по определению являются составными, 
    будем рассматривать только нечетные числа. 
    Индекс i в массиве отображает нечетное число 2i+1
    */
   
    // массив хранит флаг простоты числа (true -- число простое)
    bool* is_composite = (bool*) malloc(num_of_odd_number*sizeof(bool));
    std::fill_n(is_composite, num_of_odd_number, true); // изначально помечаем все числа, как потенциально простые

    std::vector<int> prime_steps = get_first_batcn_prime_numbers(is_composite, batch_size);

    auto start_positions = get_all_start_positions(prime_steps, batch_size);

    auto exec_time = parallel_sieve(prime_steps, start_positions, batch_size, is_composite);

    #if VALIDATION
    print_prime_numbers_to_file(res_file_path, is_composite, N);
    #endif

    #if TIME
    FILE* fd = fopen(time_file_path, "a");
    fprintf(fd, "%lf ", exec_time);
    fclose(fd);
    #endif

    free(is_composite);
    return 0;
}