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

    
        //N = 2000 / Thread = 10 Time = 91 sec  / Usuall Time = 87 sec
       //N = 2000 / Thread = 4 Time  94 sec    / Usuall Time = 90 sec
      //N = 1500 / Thread = 100 Time  36 sec  / Usuall Time = 34 sec
     //N = 1500 / Thread = 10 Time  36 sec   / Usuall Time = 35 sec
    //N = 1500 / Thread = 4 Time  33 sec    / Usuall Time = 35 sec
   //N = 1000 / Thread = 1000 Time = 9 sec / Usuall Time = 9 sec
  //N = 1000 / Thread = 100 Time = 9 sec  / Usuall Time = 9 sec
 //N = 1000 / Thread = 10 Time = 9 sec   / Usuall Time = 9 sec
//N = 1000 / Thread = 4 Time = 10 sec   / Usuall Time = 11 sec

#define N 1500
#define RANGE 20

struct Struct {
    double** matrix_1Pointer;
    double** matrix_2Pointer;
    double** matrix_3Pointer;
    int begin;
    int end;
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

void getResultMatrix(struct Struct args) {
    double* Hor = (double*) calloc(N, sizeof(double));
    double* Vert = (double*) calloc(N, sizeof(double));
    for (int i = args.begin; i < args.end; i++) {   //correct
        for (int j = 0; j < N; j++) {
            getHor(&Hor, *(args.matrix_1Pointer), i);
            getVert(&Vert, *(args.matrix_2Pointer), j);
            *(*(args.matrix_3Pointer) + i * N + j) = multpVect(Hor, Vert);
            //printf("%5.0f ", *(*(args.matrix_3Pointer) + i * N + j));
        }
        //printf("\n");
    }
}

void *thread(void* args1) {
    struct Struct *args = (struct Struct *)args1;
    getResultMatrix(*args);
}

int main() {
    struct Struct *matrixes;
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
    
    printf("Enter number of division on threads ");
    scanf("%d", &numbThreads);
   
    matrixes = (struct Struct*) calloc(numbThreads, sizeof(struct Struct));
    numbCount = N/numbThreads;
 
    for (i = 0; i < numbThreads; i++) {
        matrixes[i].matrix_1Pointer = &matrix_1;
        matrixes[i].matrix_2Pointer= &matrix_2;
        matrixes[i].matrix_3Pointer = &matrix_3;
    }
    
    pthread_t thid[numbThreads];
    int clock1 = clock();
    int n = 0;
    
    for (i = 0; i < numbCount * numbThreads; i++) {
        matrixes[n].begin = i;
        matrixes[n].end = i + numbCount;
        int result = pthread_create((thid+n), (pthread_attr_t *) NULL, thread, matrixes+n);
        n++;
        i = i + numbCount - 1;
    }
    
    for (i = 0; i < numbThreads; i++) {
       pthread_join(thid[i], (void **) NULL);
    }
    

    printf("Time %ld", (clock() - clock1)/CLOCKS_PER_SEC);
    printf("\n");
    clock1 = clock();

    matrixes[0].begin = 0;
    matrixes[0].end = N;

    getResultMatrix(matrixes[0]);

    printf("Time %ld\n", (clock() - clock1)/CLOCKS_PER_SEC);
    free(matrix_1);
    free(matrix_2);
    free(matrix_3);
    return 0;
}
// TODO: приведите замеры ускорения в зависомости от кол-ва нитей для больших матриц, например 1000 на 1000
