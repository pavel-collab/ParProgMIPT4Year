//! This example is not correct. Author couldn't to achive real nested task parallelism.
//! To view nested tasks execution result you need to be able to stop some threads during their work.

#include <iostream>
#include <omp.h>
#include <unistd.h>

#define LARGE_NUMBER 1000000

extern void UslessTaskFunction(int task_id, int thead_rank) {
    printf("Out: %d\n", task_id);
    /*
    Some kind of independent operations, for example, some mathematical operations.
    It may used in a dynamic programming, when you divide your problem to a lo of 
    independent (or almost independing) tasks, and it doesn't matter, what thread 
    executes this task.
    */

    //* Atrificial timeout
    // for (auto _ = 0; _ < 10000000; ++_) {}
    sleep(1000);
    printf("Task [%d] is executed by thread rank: %d\n", task_id, thead_rank);
}

extern void PotentialDangerousUslessTaskFunction(int task_id, int thead_rank) {
    printf("Task [%d] is executed by thread rank: %d\n", task_id, thead_rank);
    /*
    Some kind of operations, that uses an internal states of thread (for example id of
    private variable).
    */
    //* Atrificial timeout
    // for (auto _ = 0; _ < 10000000; ++_) {}
    sleep(1000);
    printf("Task [%d] is executed by thread rank: %d\n", task_id, thead_rank);
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "[-] Usage %s n -- the number of threads\n", argv[0]);
        return -1;
    }

    uint32_t threads_num = atoi(argv[1]);

    // set the number of threads
    omp_set_num_threads(threads_num);
    // start the parallel section
    /*
    Here we make the one thread to generate a lot of utied tasks in a single
    scope. That means, that all of the tasks will be produced by one thread, but
    may be executed by another. In this case we will have a potential dengerous
    program behaviour for example if the function that executed in a task uses
    some kinds of variables special for the thread, that created this task. 
    It may be thread id or thread privet variables.
    */
    #pragma omp parallel
    {
        #pragma omp single
        {
            int i = 0;
            #pragma omp task untied
            {
                /*
                We just spawn a lot of tasks in a single scope to
                overflow the tasks pull and make master thread to stop to do some tasks
                and give it to another threads for execution.
                */
                for (i = 0; i < LARGE_NUMBER; i++) {
                    printf("Task %d was born by thread %d\n", i, omp_get_thread_num());
                    #pragma omp task
                    {
                        PotentialDangerousUslessTaskFunction(i, omp_get_thread_num());
                    }
                }
            }
        }
    }

    return 0;
}