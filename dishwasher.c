#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <errno.h>
#include <unistd.h>

char* fifo1 = "7.txt";
char* fifo2 = "8.txt";


void funcGivePerm1(int id, int token) {
    struct sembuf mybuf1; 
    mybuf1.sem_op = token;
    mybuf1.sem_flg = 0;
    mybuf1.sem_num = 0;

    if(semop(id, &mybuf1, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }
}

void funcGivePerm2(int id, int token) {
    struct sembuf mybuf2; 
    mybuf2.sem_op = token;
    mybuf2.sem_flg = 0;
    mybuf2.sem_num = 0;

    if(semop(id, &mybuf2, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }
}



void *CleaningThread(void* dirty_dish) {
    int fd1 = open(fifo1, O_WRONLY);
    int fd2 = open(fifo2, O_WRONLY);
    int i = 1;
    while (i < *((int *)dirty_dish + 0)) {
        //printf("wet\n");
        funcGivePerm1(fd1, -1);
        sleep(*((int *)dirty_dish + i));
        funcGivePerm2(fd2, 1);
    }
}

void *DryingThread(void* wet_dish) {
    int fd1 = open(fifo1, O_WRONLY);
    int fd2 = open(fifo2, O_WRONLY);
    int i = 1;
    while (i < *((int *)wet_dish + 0)) {
        //printf("dry\n");
        funcGivePerm2(fd2, -1);
        sleep(*((int *)wet_dish + i));
        funcGivePerm1(fd1, 1);
    }

}

void getDishTime(char *source, int **dish_time) {
    FILE* fp = fopen(source, "r");
    char buf[1000];
    int space = ' ';
    int i = 1;
    while (fgets(buf, 1000, fp) != NULL) {  
        *dish_time = (int *) realloc(*dish_time, (i + 2) * sizeof(int));
        *(*dish_time + i) = atoi(strchr(buf, space));
        i++;
    }
    **dish_time = i - 1;
}

int main() {
    char *dirty_dish_source = "dirty_dish.txt";
    char *wet_dish_source = "wet_dish.txt";
    int *dirty_dish;
    int *wet_dish;
    int i, table_limit, semid1, semid2;
    dirty_dish = (int *) malloc((1) * sizeof(int));
    wet_dish = (int *) malloc((1) * sizeof(int));
    scanf("%d", &table_limit);
    
    getDishTime(dirty_dish_source, &dirty_dish);
    getDishTime(wet_dish_source, &wet_dish);
    
    
    pthread_t thid1, thid2;
    key_t key1, key2; 
    struct sembuf mybuf1;
    struct sembuf mybuf2; 

    if((key1 = ftok(fifo1,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((key2 = ftok(fifo2,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((semid1 = semget(key1, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }
    if((semid2 = semget(key2, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }
    
    funcGivePerm1(semid1, table_limit);
	int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, CleaningThread, dirty_dish);
    funcGivePerm2(semid2, 0);
    int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, DryingThread, wet_dish);
    
    pthread_join(thid1, (void **)NULL);
    pthread_join(thid2, (void **)NULL);

    free(dirty_dish);
    free(wet_dish);
    return 0;
}