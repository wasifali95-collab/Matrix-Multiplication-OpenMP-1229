#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// Function to multiply matrices sequentially
void multiply_matrices(int** A, int** B, int** C, int size) {
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

    int sizes[] = {200, 500, 1000}; // Three matrix sizes
    int num_threads[] = {1, 4, 8};  // Threads used (for reference, even in sequential)

    for (int s = 0; s < 3; s++) {
        int size = sizes[s];
        printf("\n=== Matrix Size: %dx%d | Threads: %d (Sequential) ===\n", size, size, num_threads[s]);

        // Allocate memory
        int** A = allocate_matrix(size);
        int** B = allocate_matrix(size);
        int** C = allocate_matrix(size);

        // Initialize matrices
        initialize_matrix(A, size);
        initialize_matrix(B, size);

        double total_time = 0.0;

        // Run the sequential version multiple times
        for (int run = 0; run < RUNS; run++) {
            clock_t start = clock();
            multiply_matrices(A, B, C, size);
            clock_t end = clock();
            double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
            total_time += time_taken;
            printf("Run %d: Execution Time = %f seconds\n", run + 1, time_taken);
        }

        double avg_time = total_time / RUNS;
        printf("Average Execution Time (Sequential) = %f seconds\n", avg_time);

        // Free allocated memory
        free_matrix(A, size);
        free_matrix(B, size);
        free_matrix(C, size);
    }

    return 0;
}
