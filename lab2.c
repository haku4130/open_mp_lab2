#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int parallel_program(int threads, int random_seed, double *time) {
    const int count = 10000000;     ///< Number of array elements
//    const int random_seed = 920214; ///< RNG seed
    const int target = 16;          ///< Number to look for

    int *array;                 ///< The array we need to find the max in
    int index = -1;                ///< The index of the element we need

    /* Initialize the RNG */
    srand(random_seed);

    /* Generate the random array */
    array = (int *) malloc(count * sizeof(int));
    for (int i = 0; i < count; i++) { array[i] = rand(); }


    double start_time = omp_get_wtime();
    /* Find the index of the element */
#pragma omp parallel num_threads(threads) shared(index)
    {
#pragma omp for
        for (int i = 0; i < count && index == -1; i++) {
            if (array[i] == target) {
#pragma omp critical // нужна ли теперь critical
                {
                    index = i;
                }
            }
        }
    }
    double end_time = omp_get_wtime();
    *time = end_time - start_time;

    printf("On %d thread(s) found occurrence of %d at index %d;\n", threads, target, index);
    return (index);
}


int main() {
    int res[16][10];
    double time[160];
    for (int i = 1; i <= 16; i++) {
        int random_seed = 920214;
        for (int j = 0; j < 10; j++) {
            res[i - 1][j] = parallel_program(i, random_seed + j, &time[i + j - 1]);
        }
    }
    for (int i = 0; i < 160; i++){
        printf("%f", time[i]);
    }
    return 0;
}