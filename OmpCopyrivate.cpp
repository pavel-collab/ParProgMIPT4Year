#include <stdio.h>
#include <omp.h>

float x, y, fGlobal = 1.0;
#pragma omp threadprivate(x, y)

float get_float() {
    fGlobal += 0.001;
    return fGlobal;
}

void use_float(float f, int t) {
    printf("Value = %f, thread = %d\n", f, t);
}

void CopyPrivate(float a, float b) {
    #pragma omp single copyprivate(a, b, x, y)
    {
        a = get_float();
        b = get_float();
        x = get_float();
        y = get_float();
        }

    use_float(a, omp_get_thread_num());
    use_float(b, omp_get_thread_num());
    use_float(x, omp_get_thread_num());
    use_float(y, omp_get_thread_num());
}

int main() {
    float a = 9.99, b = 123.456;

    printf("call CopyPrivate from a single thread\n");
    CopyPrivate(9.99, 123.456);

    printf("call CopyPrivate from a parallel region\n");
    #pragma omp parallel
    {
        CopyPrivate(a, b);
    }
}