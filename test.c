#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//gcc 1lab.c -o 1lab.exe
// start 1lab.exe 5

void dgemm_blas(int N, double **A, double **B, double **C); // Функция для умножения двух квадратных матриц
double** allocate_matrix(int N); // Функция для выделения памяти для матрицы
void initialize_matrix(int N, double **matrix);// Функция для инициализации матрицы случайными значениями
void print_matrix(int N, double **matrix); // Функция для вывода матрицы на экран








int main(int argc, char *argv[]) {
    // Проверяем, передано ли нужное количество аргументов
    if (argc != 2) {
        printf("Usage: %s <size_of_matrix>\n", argv[0]);
        getchar();
        return 1;
    }

    // Получаем размер матрицы из аргументов командной строки
    int N = atoi(argv[1]);

    // Инициализируем генератор случайных чисел
    srand(time(NULL));

    // Выделяем память для матриц
    double **A = allocate_matrix(N);
    double **B = allocate_matrix(N);
    double **C = allocate_matrix(N);

    // Инициализируем матрицы случайными значениями
    initialize_matrix(N, A);
    initialize_matrix(N, B);

    clock_t start_time = clock(); // старт секундомера
    dgemm_blas(N, A, B, C); // Выполняем умножение матриц
    clock_t end_time = clock(); // Замер времени после умножения
    double time_spent = (double)(end_time - start_time) / CLOCKS_PER_SEC; // Вычисляем затраченное время
    printf("Matrix multiplication of size %d took %f seconds.\n", N, time_spent); // Выводим время, затраченное на умножение

    // Освобождаем память
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

void dgemm_blas(int N, double **A, double **B, double **C) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            C[i][j] = 0.0;
            for (int k = 0; k < N; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

// Функция для выделения памяти для матрицы
double** allocate_matrix(int N) {
    double **matrix = (double **)malloc(N * sizeof(double *));
    for (int i = 0; i < N; i++) {
        matrix[i] = (double *)malloc(N * sizeof(double));
    }
    return matrix;
}

// Функция для инициализации матрицы случайными значениями
void initialize_matrix(int N, double **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            matrix[i][j] = ((double)rand() / (double)(RAND_MAX)) * 10.0;
        }
    }
}

// Функция для вывода матрицы на экран
void print_matrix(int N, double **matrix) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%f ", matrix[i][j]);
        }
        printf("\n");
    }
}