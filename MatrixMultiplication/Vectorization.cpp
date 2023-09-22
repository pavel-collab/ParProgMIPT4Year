#include <iostream>
#include <x86intrin.h>

int main(int argc, char* argv[]) {
    float a[8] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
    float b[8] = {2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
    
    // загружаем 8 чисел float (по 32 бита) в регистр на 256 бит
    __m256 va = _mm256_loadu_ps(a);
    __m256 vb = _mm256_loadu_ps(b);

    // умножаем 2 регистра по 256 бит, получая таким образом перемножение каждой 32-битной ячейки
    __m256 temp = _mm256_mul_ps(va, vb);

    // извлекаем 128 старших битов в отдельную переменную
    __m128 vhigh1 = _mm256_extractf128_ps(temp, 1);
    // у регистра temp отбраысваем старшие биты
    // таким образом, мы разделили регистр temp на 2 регистра по 128 бит
    __m128 vhigh2 = _mm256_castps256_ps128(temp);

    // складываем значения всех 32-битных ячеек
    __m128 result = _mm_add_ps(vhigh1, vhigh2);

    result = _mm_hadd_ps(result, result);
    result = _mm_hadd_ps(result, result);

    // в конце мы получаем 128-битный регистр, где каждое 32-битное значение -- наш искомый ответ

    // кастуем младшие 32 бита в float и копируем результат в переменную res
    float res = _mm_cvtss_f32(result);

    printf("resilt is %lf\n", res);
    return 0;
}