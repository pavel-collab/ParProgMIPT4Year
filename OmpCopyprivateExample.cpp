#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char* argv[])
{
	int a = 123;

    printf("======================\n");
    printf("first parallel section\n");
    printf("======================\n");
	#pragma omp parallel default(none) firstprivate(a)
	{
		printf("Thread %d: a = %d.\n", omp_get_thread_num(), a);
		#pragma omp barrier

        /*
        Благодаря copyprivate, не смотря на то, что значение переменной менялось 
        только в одном потоке, итоговый результат (новое значение переменной), 
        наблюдается всеми потоками.
        */
		#pragma omp single copyprivate(a)
		{
			a = 456;
			printf("\nThread %d executes the single construct and changes a to %d.\n\n", omp_get_thread_num(), a);
		}

		printf("Thread %d: a = %d.\n", omp_get_thread_num(), a);
	}

    printf("\n");
    printf("=======================\n");
    printf("second parallel section\n");
    printf("=======================\n");

    #pragma omp parallel default(none) firstprivate(a)
	{
		printf("Thread %d: a = %d.\n", omp_get_thread_num(), a);

		#pragma omp barrier

		#pragma omp single
		{
			a = 456;
			printf("\nThread %d executes the single construct and changes a to %d.\n\n", omp_get_thread_num(), a);
		}

		printf("Thread %d: a = %d.\n", omp_get_thread_num(), a);
	}

	return EXIT_SUCCESS;
}