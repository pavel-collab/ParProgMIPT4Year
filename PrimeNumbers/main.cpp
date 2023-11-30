#include <iostream>
#include <cmath>
#include <omp.h>
#include <chrono>
#include <algorithm> 
#include <vector>

// Процедура strike вычеркивает составные числа в интервале [start; limit) с шагом step.
// Возвращает значение limit%step==0? limit: limit-step.
void strike(int start, int limit, int step, bool* is_composite) {
    for (; start<= limit; start += step)
        is_composite[start] = false;
}

int main(int argc, char* argv[]) {
    // в параметрах программы задаем N и количество потоков
    if (argc < 3) {
        fprintf(stderr, "Usage %s N proces_number", argv[0]);
        return -1;
    }
    
    int N = atoi(argv[1]);
    int proces_number = atoi(argv[2]);

    // set the number of threads
    omp_set_num_threads(proces_number);

    // вычисляем размер поддиапазона
    int batch_size = sqrt(N);
    // printf("[DEBUG] batch size is %d\n", batch_size);
    batch_size += batch_size&1;

    // поскольку четные числа по определению являются составными, 
    // будем рассматривать только нечетные числа. 
    // Индекс i в массиве отображает нечетное число 2i+1
    
    // хранит флаг простоты числа
    int tmp = N/2;
    bool* is_composite = (bool*) malloc(tmp*sizeof(bool));
    std::fill_n(is_composite, N/2, true); // изначально помечаем все числа, как потенциально простые

    // хранит простые числа, найденные в самом первом поддиапазоне.
    // Эти числа ищутся обычным алгоритмом
    std::vector<int> prime_steps;

    // число найденных на первом этапе простых чисел
    int n_prime_steps = 0;

    // Найдем все простые числа в первом поддиапазоне с помощью классического алгоритма
    for (int i = 3; i <= batch_size; i += 2)  {
        // если число простое
        if(is_composite[i/2]) {
            // с шагом равному простому числу перемещаемся по интервалу [простое число/2; m/2]
            // в результате вычеркнем все числа, делящиеся на простое число i (модифицируем массив is_composite)
            strike(i/2+i, batch_size/2, i, is_composite);
            // запомним шаг\новое простое число
            prime_steps.emplace_back(i);
            n_prime_steps++;
        }
    }   

    // printf("[DEBUG] number of prime numbers in the first batch is %d\n", n_prime_steps);

    // for (size_t i = 0; i < n_prime_steps; ++i) {
    //     std::cout << prime_steps[i] << std::endl;
    // }

    std::vector<std::vector<int>> start_positions;
    for (auto item : prime_steps) {
        std::vector<int> tmp;
        for (int i = 1; i < batch_size; ++i) {
            int start = i*batch_size;
            int p = start % item;

            int l = 0;
            if (p%2 == 0) {
                l = start + (item - p);
            } else {
                l = start + (2*item - p);
            }

            int pos = (l-1) / 2;
            tmp.emplace_back(pos);
        }
        // std::cout << "[DEBUG] prime = " << item << std::endl;
        // for (auto item : tmp) {
        //     std::cout << "[DEBUG] " << item << " ";
        // }
        // std::cout << std::endl;
        start_positions.emplace_back(tmp);
    }

    auto t_start = std::chrono::high_resolution_clock::now();
    #pragma omp parallel for schedule(static,1) shared(is_composite)
    // начинаем обходить массив чисел (шагаем целыми батчами) i -- нормер батча
    for (int i = 1; i < batch_size; i++) {
        // для выбранного батча перебираем простые числа, 
        // смотрим позицию, с которой надо начать вычеркавание в этом батче для этого простого числа
        for (int j = 0; j < n_prime_steps; ++j) {
            auto prime = prime_steps[j];
            auto start = start_positions[j][i-1];

            // printf("[DEBUG] batch = %d, prime = %d, start = %d, end = %d\n", i, prime, start, ((i+1)*batch_size - 1) / 2);

            strike(start, ((i+1)*batch_size - 1) / 2, prime, is_composite);
        }
    }
    auto t_end = std::chrono::high_resolution_clock::now();

    std::cout << "time is " << std::chrono::duration<double, std::milli>(t_end-t_start).count() << " ms" << std::endl;

    // std::cout << std::endl;
    // for (int i = 0; i < N/2; ++i) {
    //     if (is_composite[i]) {
    //         std:: cout << 2*i + 1 << std::endl;
    //     }
    // }

    free(is_composite);
    return 0;
}