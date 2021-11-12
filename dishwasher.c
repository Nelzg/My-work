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
struct DishInfo{
    int *dish_time;
    char **dish_type;
    int dish_numb;
    int semid1;
    int semid2;
};

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
    struct DishInfo *dirty = (struct DishInfo *)dirty_dish;
    int i = 0;
    while (1) {
        funcGivePerm1((*dirty).semid1, -1);
        printf("cleaned ");
        //sleep((*dirty).dish_time[i]);
        printf("%s\n",(*dirty).dish_type[i]);
        funcGivePerm2((*dirty).semid2, 1);
        printf("passed\n");
        i++;
    }
}

void *DryingThread(void* wet_dish) {
    struct DishInfo *wet = (struct DishInfo *)wet_dish;
    int i = 1;
    while (1) {
        funcGivePerm2((*wet).semid2, -1);
        printf("dryed ");
        //sleep((*wet).dish_time[i]);
        printf("%s\n",(*wet).dish_type[i]);
        funcGivePerm1((*wet).semid1, 1);
        printf("passed\n");
        i++;
    }

}

int getDishInfo(char *source, int **dish_time, char ***dish_type) {
    FILE* fp = fopen(source, "r");
    char buf1[1000];
    long int numb;
    int space = ' ';
    int i = 0;
    while (fgets(buf1, 1000, fp) != NULL) {  
        *dish_time = (int *) realloc(*dish_time, (i + 1) * sizeof(int));
        *dish_type = (char **) realloc(*dish_type, (i + 1) * sizeof(char*));
        numb = strchr(buf1, space) - buf1 - 1;
        *(*dish_type + i) = (char *) malloc(numb * sizeof(char));
        strncpy(*(*dish_type + i),buf1, numb);
        *(*dish_time + i) = atoi(strchr(buf1, space));
        *(*(*dish_type + i) + numb) = '\0';
        i++; 
    }
    return i - 1;
}

int main() {
    char *dirty_dish_source = "dirty_dish.txt";
    char *wet_dish_source = "wet_dish.txt";
    struct DishInfo dirty,wet;
    int i, table_limit, semid1, semid2;
    dirty.dish_time = (int *) malloc((1) * sizeof(int));
    wet.dish_time = (int *) malloc((1) * sizeof(int));
    dirty.dish_type = (char **) malloc((1) * sizeof(char *));
    wet.dish_type = (char **) malloc((1) * sizeof(char *));

    scanf("%d", &table_limit);
    dirty.dish_numb = getDishInfo(dirty_dish_source, &dirty.dish_time, &dirty.dish_type);
    wet.dish_numb = getDishInfo(wet_dish_source, &wet.dish_time, &wet.dish_type);

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
    dirty.semid1 = semid1;
    dirty.semid2 = semid2;
    wet.semid1 = semid1;
    wet.semid2 = semid2;
    funcGivePerm1(semid1, table_limit);
	int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, CleaningThread, &dirty.dish_time);
    funcGivePerm2(semid2, 0);
    int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, DryingThread, &wet.dish_time);
    
    pthread_join(thid1, (void **)NULL);
    pthread_join(thid2, (void **)NULL);
    semctl(semid1, 0, IPC_RMID);
    semctl(semid2, 1, IPC_RMID);
    free(dirty.dish_time);
    free(wet.dish_time);
    for (i = 0; i < dirty.dish_numb; i++) {
        free(dirty.dish_type[i]);
    }
    free(dirty.dish_type);
    for (i = 0; i < wet.dish_numb; i++) {
        free(wet.dish_type[i]);
    }
    free(wet.dish_type);
    return 0;
}