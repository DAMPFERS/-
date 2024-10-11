#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Глобальные переменные для доступа потоков
int N, num_threads;
double **A, **B, **C;

// Функция для выделения памяти для матрицы
double** allocate_matrix(int N) {
    double **matrix = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (double *)malloc(N * sizeof(double));
    }
    return matrix;
}

// Функция для инициализации матрицы случайными числами (используется в потоках)
void* initialize_matrix(void* arg) {
    int thread_id = *(int*)arg;
    int chunk_size = N / num_threads;
    int start = thread_id * chunk_size;
    int end = (thread_id == num_threads - 1) ? N : start + chunk_size;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            A[i][j] = ((double)rand() / (double)(RAND_MAX)) * 10.0;
            B[i][j] = ((double)rand() / (double)(RAND_MAX)) * 10.0;
        }
    }
    return NULL;
}

// Функция для умножения матриц (используется в потоках)
void* dgemm_blas(void* arg) {
    int thread_id = *(int*)arg;
    int chunk_size = N / num_threads;
    int start = thread_id * chunk_size;
    int end = (thread_id == num_threads - 1) ? N : start + chunk_size;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <size_of_matrix> <num_threads>\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    num_threads = atoi(argv[2]);

    srand(time(NULL));

    A = allocate_matrix(N);
    B = allocate_matrix(N);
    C = allocate_matrix(N);

    pthread_t threads[num_threads];
    int thread_ids[num_threads];

    // Инициализация матриц в потоках
    for (int i = 0; i < num_threads; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, initialize_matrix, &thread_ids[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    // Измеряем время умножения матриц
    clock_t start_time = clock();
    
    // Умножение матриц в потоках
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, dgemm_blas, &thread_ids[i]);
    }
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Matrix multiplication of size %d with %d threads took %f seconds.\n", N, num_threads, time_spent);

    for (int i = 0; i < N; i++) {
        free(A[i]);
        free(B[i]);
        free(C[i]);
    }
    free(A);
    free(B);
    free(C);

    return 0;
}
