#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <time.h>

#define N 3
#define RANGE 20

struct Struct {
    double** matrix_1Pointer;
    double** matrix_2Pointer;
    double** matrix_3Pointer;
    int begin,end;
};

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

void getResultMatrix(struct Struct* args, double* Hor, double* Vert) {
    for (int i = args->begin; i < args->end; i++) {
        for (int j = 0; j < N; j++) {
            getHor(&Hor, *(args -> matrix_1Pointer), i);
            getVert(&Vert, *(args -> matrix_2Pointer), j);
            *(*(args -> matrix_3Pointer) + i * N + j) = multpVect(Hor, Vert);
            printf("%5.0f ", *(*(args -> matrix_3Pointer) + i * N + j));
        }
        printf("\n");
    }
}

void *thread(struct Struct* args) {
    double* Hor = (double*) calloc(N, sizeof(double));
    double* Vert = (double*) calloc(N, sizeof(double));
    getResultMatrix(args, Hor, Vert);
}

int main() {
    struct Struct* Matrixes;
    int numbThreads;
    int numbCount;
    double* matrix_1;
    double* matrix_2;
    double* matrix_3;
    double* Vert;
    double* Hor;
    int i, j, k;

    matrix_1 = (double*) calloc(N * N, sizeof(double));
    matrix_2 = (double*) calloc(N * N, sizeof(double));
    matrix_3 = (double*) calloc(N * N, sizeof(double));
    Hor = (double*) calloc(N, sizeof(double));
    Vert = (double*) calloc(N, sizeof(double));

    srand(time(NULL));

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            *(matrix_1 + i * N + j) = (random() % RANGE) * pow(-1, rand() % 2);
            *(matrix_2 + i * N + j) = (random() % RANGE) * pow(-1, rand() % 2);
            //printf("%3.0f ", *(matrix_1 + i * N + j));
        }
        //printf(" | ");
        for (int j = 0; j < N; j++) {
            //printf("%3.0f ", *(matrix_2 + i * N + j));
        }
        //printf("\n");
    }

    Matrixes->matrix_1Pointer = &matrix_1;
    Matrixes->matrix_2Pointer= &matrix_2;
    Matrixes->matrix_3Pointer = &matrix_3;

    printf("Enter number of division on threads ");
    scanf("%d", &numbThreads);
    numbCount = N/numbThreads;
    
    int clock1 = clock();
    for (i = 0; i < numbCount * numbThreads; i++) {
        pthread_t thid;
        Matrixes -> begin = i;
        Matrixes -> end = i + numbCount;
        int result = pthread_create(&thid, (pthread_attr_t *) NULL, thread, Matrixes);
        pthread_join(thid, (void **) NULL);
        i = i + numbCount - 1;
    }
    printf("Time %ld", clock() - clock1);
    printf("\n");
    clock1 = clock();

    Matrixes -> begin = 0;
    Matrixes -> end = N;

    getResultMatrix(Matrixes, Hor, Vert);

    printf("Time %ld\n", clock() - clock1);
    free(matrix_1);
    free(matrix_2);
    free(matrix_3);
    free(Vert);
    free(Hor);
    return 0;
}
// TODO: приведите замеры ускорения в зависомости от кол-ва нитей для больших матриц, например 1000 на 1000
