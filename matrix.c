#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4

void getHor(int** buf,int** matrix, int i) {
    int k = 0;
    for (int j = 0; j < N; j++) {
        *(*buf+k) = *(*matrix + i * N + j);
        k++;
    }
}

void getVert(int** buf,int** matrix, int j) {
    int k = 0;
    for (int i = 0; i < N; i++) {
        *(*buf+k) = *(*matrix + i * N + j);
        k++;
    }
}

int multpVect(int* Hor, int* Vert ) {
    int n = 0;
    for (int j = 0; j < N; j++) {
        n = n + Hor[j] * Vert[j];
    }
    return n;
}

int main() {
    int** a;
    int** b;
    int** c;
    int* Vert;
    int* Hor;
    int i, j, k;
    k = 1;
    a = (int**) calloc(N, sizeof(int*));
    b = (int**) calloc(N, sizeof(int*));
    c = (int**) calloc(N, sizeof(int*));
    Hor = (int*) calloc(N, sizeof(int));
    Vert = (int*) calloc(N, sizeof(int));
    for (i = 0; i < N; i++) {
        a[i] = (int*) calloc(N, sizeof(int));
        b[i] = (int*) calloc(N, sizeof(int));
        c[i] = (int*) calloc(N, sizeof(int));
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            *(*a + i * N + j) = k;
            *(*b + i * N + j) = k;
            k++;
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            getHor(&Hor, a, i);
            getVert(&Vert, a, j);
            for (int k = 0; k < N; k++) {
                printf("(%d) ", Vert[k]);
                printf("%d ", Hor[k]);
            }
            printf("\n");
            *(*c + i * N + j) = multpVect(Hor, Vert);
            printf("%d\n ", *(*c + i * N + j));
        }
        printf("\n");
    }
    return 0;
}