//OpenMP
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <time.h>

double** allocate_matrix(int N) {
    double **matrix = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (double *)malloc(N * sizeof(double));
    }
    return matrix;
}

void initialize_matrix(int N, double **matrix) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = ((double)rand() / (double)(RAND_MAX)) * 10.0;
        }
    }
}

void dgemm_blas(int N, double **A, double **B, double **C) {
    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <size_of_matrix> <num_threads>\n", argv[0]);
        getchar();
        return 1;
    }

    int N = atoi(argv[1]);
    int num_threads = atoi(argv[2]);

    srand(time(NULL));

    double **A = allocate_matrix(N);
    double **B = allocate_matrix(N);
    double **C = allocate_matrix(N);

    omp_set_num_threads(num_threads);

    initialize_matrix(N, A);
    initialize_matrix(N, B);

    double start_time = omp_get_wtime();
    
    dgemm_blas(N, A, B, C);
    
    double end_time = omp_get_wtime();
    double time_spent = end_time - start_time;

    printf("Matrix multiplication of size %d with %d threads took %f seconds.\n", N, num_threads, time_spent);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);
    getchar();
    return 0;
}
