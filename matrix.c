#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define N 3
#define RANGE 100

void getHor(double** Hor,double* matrix, int i) {
    int k = 0;
    for (int j = 0; j < N; j++) {
        *(*Hor + k) = *(matrix + i * N + j);
        k++;
    }
}

void getVert(double** Vert,double* matrix, int j) {
    int k = 0;
    for (int i = 0; i < N; i++) {
        *(*Vert + k) = *(matrix + i * N + j);
        k++;
    }
}

int multpVect(double* Hor, double* Vert ) {
    int n = 0;
    for (int j = 0; j < N; j++) {
        n = n + Hor[j] * Vert[j];
    }
    return n;
}

int main() {
    double* matrix_1;
    double* matrix_2;
    double* matrix_3;
    double* Vert;
    double* Hor;
    int i, j, k;
    k = 1;
    matrix_1 = (double*) calloc(N *N, sizeof(double));
    matrix_2 = (double*) calloc(N * N, sizeof(double));
    matrix_3 = (double*) calloc(N * N, sizeof(double));
    Hor = (double*) calloc(N, sizeof(double));
    Vert = (double*) calloc(N, sizeof(double));

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            *(matrix_1 + i * N + j) = (random() % RANGE) * pow(-1, rand() % 2);
            *(matrix_2 + i * N + j) = (random() % RANGE) * pow(-1, rand() % 2);
            k++;
            printf("%3.0f ", *(matrix_1 + i * N + j));
        }
        printf(" | ");
        for (int j = 0; j < N; j++) {
            printf("%3.0f ", *(matrix_2 + i * N + j));
        }
        printf("\n");
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            getHor(&Hor, matrix_1, i);
            getVert(&Vert, matrix_1, j);
            *(matrix_3 + i * N + j) = multpVect(Hor, Vert);
            printf("%5.0f ", *(matrix_3 + i * N + j));
        }
        printf("\n");
    }
    free(matrix_1);
    free(matrix_2);
    free(matrix_3);
    free(Vert);
    free(Hor);
    return 0;
}