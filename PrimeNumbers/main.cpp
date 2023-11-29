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
    batch_size += batch_size&1;

    // поскольку четные числа по определению являются составными, 
    // будем рассматривать только нечетные числа. 
    // Индекс i в массиве отображает нечетное число 2i+1
    
    // хранит флаг простоты числа
    bool* is_composite = new bool(N/2);
    std::fill_n(is_composite, N/2, true); // изначально помечаем все числа, как потенциально простые

    // хранит простые числа, найденные в самом первом поддиапазоне.
    // Эти числа ищутся обычным алгоритмом
    //TODO: заменить на вектор
    std::vector<int> prime_steps;
    std::fill_n(is_composite, N/2, 0);

    // число найденных на первом этапе простых чисел
    int n_prime_steps = 0;

    // Найдем все простые числа в первом поддиапазоне с помощью классического алгоритма
    for (int i = 3; i <= batch_size; i += 2)  {
        // если число простое
        if(is_composite[i/2]) {
            // с шагом равному простому числу перемещаемся по интервалу [простое число/2; m/2]
            // в результате вычеркнем все числа, делящиеся на простое число i (модифицируем массив is_composite)
            strike(i/2, batch_size/2, i, is_composite);
            // запомним шаг\новое простое число
            prime_steps.emplace_back(i);
            n_prime_steps++;
        }
    }   

    for (auto item : prime_steps) {
        std::cout << item << std::endl;
    }

    std::vector<std::vector<int>> start_positions;
    for (auto item : prime_steps) {
        std::vector<int> tmp;
        for (int i = 1; i < batch_size; ++i) {
            int k = ((batch_size*i-1) / item) * item;
            int p = k % batch_size;
            
            int l = 0;
            if (p%2 == 0) {
                l = p + item;
            } else {
                l = p + 2*item;
            }
            tmp.emplace_back(l);
        }
        start_positions.emplace_back(tmp);
    }

    #pragma omp parallel for schedule(static,1) shared(is_composite)
    for (int i = batch_size; i < N; i+=batch_size) {
        for (int j = 0; j < start_positions.size(); ++j) {
            auto prime = prime_steps[j];
            auto start = start_positions[j][i];

            strike(start, i*batch_size/2, prime, is_composite);
        }
    }

    for (int i = 0; i <  batch_size/2; ++i) {
        if (is_composite[i]) {
            std:: cout << 2*i + 1 << std::endl;
        }
    }

    delete[] is_composite;
    return 0;
}