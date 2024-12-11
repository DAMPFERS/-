#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tbb/tbb.h>

const int N = 1000; // Размер квадратных матриц

// Функция для инициализации матриц случайными числами типа double
void initialize_matrix(double** matrix) {
    srand((unsigned int)time(NULL));  // Инициализация генератора случайных чисел

    tbb::parallel_for(0, N, [&](int i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
        });
}

// Последовательное умножение матриц с многопоточной реализацией
void DGEMM_BLAS(double** A, double** B, double** C) {
    // Параллельное распределение вычислительной нагрузки по строкам матрицы
    tbb::parallel_for(0, N, [&](int i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        });
}

// Функция для создания и инициализации двумерного массива (матрицы)
double** create_matrix() {
    double** matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; ++i) {
        matrix[i] = (double*)malloc(N * sizeof(double));
    }
    return matrix;
}

// Функция для освобождения памяти, выделенной под матрицу
void free_matrix(double** matrix) {
    for (int i = 0; i < N; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    // Устанавливаем количество потоков вручную (например, 4 потока)
    int num_threads = 3;
    tbb::task_arena arena(num_threads);

    // Создание матриц A, B и C
    double** A = create_matrix();
    double** B = create_matrix();
    double** C = create_matrix();

    // Инициализация матриц случайными числами
    initialize_matrix(A);
    initialize_matrix(B);

    // Измерение времени выполнения умножения
    clock_t start_time = clock();

    // Запуск умножения матриц в арене с заданным количеством потоков
    arena.execute([&]() {
        DGEMM_BLAS(A, B, C);
        });

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // Вывод результата
    printf("Time taken to multiply matrices: %f seconds\n", duration);
    printf("Number of threads used: %d\n", num_threads);

    // Пример вывода некоторых элементов матрицы C
    printf("C[0][0] = %f\n", C[0][0]);
    printf("C[N-1][N-1] = %f\n", C[N - 1][N - 1]);

    // Освобождение памяти
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}
