#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

#define RUNS 10  // Number of runs for averaging

// Function to allocate memory dynamically for a matrix
int** allocate_matrix(int size) {
    int** matrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++)
        matrix[i] = (int*)malloc(size * sizeof(int));
    return matrix;
}

// Function to free dynamically allocated memory
void free_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++)
        free(matrix[i]);
    free(matrix);
}

// Function to initialize matrix with random values
void initialize_matrix(int** matrix, int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            matrix[i][j] = rand() % 100;
}

// Function to multiply matrices using OpenMP with dynamic thread selection
void multiply_matrices_parallel(int** A, int** B, int** C, int size, int num_threads) {
    #pragma omp parallel for num_threads(num_threads) collapse(2)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            C[i][j] = 0;
            for (int k = 0; k < size; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    srand(time(NULL));

    int sizes[] = {200, 500, 1000};  // Three matrix sizes
    int threads[] = {1, 4, 8};       // Corresponding thread counts

    for (int s = 0; s < 3; s++) {
        int size = sizes[s];
        int num_threads = threads[s];
        printf("\n=== Matrix Size: %dx%d | Threads: %d (Parallel) ===\n", size, size, num_threads);

        // Allocate memory
        int** A = allocate_matrix(size);
        int** B = allocate_matrix(size);
        int** C = allocate_matrix(size);

        // Initialize matrices
        initialize_matrix(A, size);
        initialize_matrix(B, size);

        double total_time = 0.0;

        // Run the parallel version multiple times
        for (int run = 0; run < RUNS; run++) {
            double start = omp_get_wtime();
            multiply_matrices_parallel(A, B, C, size, num_threads);
            double end = omp_get_wtime();
            double time_taken = end - start;
            total_time += time_taken;
            printf("Run %d: Parallel Execution Time = %f seconds\n", run + 1, time_taken);
        }

        double avg_time = total_time / RUNS;
        printf("Average Execution Time (Parallel) = %f seconds\n", avg_time);

        // Free allocated memory
        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}
