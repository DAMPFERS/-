#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tbb/tbb.h>

const int N = 1000; // ������ ���������� ������

// ������� ��� ������������� ������ ���������� ������� ���� double
void initialize_matrix(double** matrix) {
    srand((unsigned int)time(NULL));  // ������������� ���������� ��������� �����

    tbb::parallel_for(0, N, [&](int i) {
        for (int j = 0; j < N; ++j) {
            matrix[i][j] = (double)rand() / RAND_MAX;
        }
        });
}

// ���������������� ��������� ������ � ������������� �����������
void DGEMM_BLAS(double** A, double** B, double** C) {
    // ������������ ������������� �������������� �������� �� ������� �������
    tbb::parallel_for(0, N, [&](int i) {
        for (int j = 0; j < N; ++j) {
            C[i][j] = 0;
            for (int k = 0; k < N; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
        });
}

// ������� ��� �������� � ������������� ���������� ������� (�������)
double** create_matrix() {
    double** matrix = (double**)malloc(N * sizeof(double*));
    for (int i = 0; i < N; ++i) {
        matrix[i] = (double*)malloc(N * sizeof(double));
    }
    return matrix;
}

// ������� ��� ������������ ������, ���������� ��� �������
void free_matrix(double** matrix) {
    for (int i = 0; i < N; ++i) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    // ������������� ���������� ������� ������� (��������, 4 ������)
    int num_threads = 3;
    tbb::task_arena arena(num_threads);

    // �������� ������ A, B � C
    double** A = create_matrix();
    double** B = create_matrix();
    double** C = create_matrix();

    // ������������� ������ ���������� �������
    initialize_matrix(A);
    initialize_matrix(B);

    // ��������� ������� ���������� ���������
    clock_t start_time = clock();

    // ������ ��������� ������ � ����� � �������� ����������� �������
    arena.execute([&]() {
        DGEMM_BLAS(A, B, C);
        });

    clock_t end_time = clock();
    double duration = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    // ����� ����������
    printf("Time taken to multiply matrices: %f seconds\n", duration);
    printf("Number of threads used: %d\n", num_threads);

    // ������ ������ ��������� ��������� ������� C
    printf("C[0][0] = %f\n", C[0][0]);
    printf("C[N-1][N-1] = %f\n", C[N - 1][N - 1]);

    // ������������ ������
    free_matrix(A);
    free_matrix(B);
    free_matrix(C);

    return 0;
}
