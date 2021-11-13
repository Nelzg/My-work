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
#include <assert.h>

char* fifo1 = "7.txt";
char* fifo2 = "Table.txt";
struct DishInfo {
    int *dish_time;
    char **dish_type;
    int dish_numb1;
    int dish_numb2;
    int semid;
};

void funcGivePerm(int id, int token, int num) {
    struct sembuf mybuf; 
    mybuf.sem_op = token;
    mybuf.sem_flg = 0;
    mybuf.sem_num = num;

    if(semop(id, &mybuf, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }
}

void *CleaningThread(void* dirty_dish) {
    struct DishInfo *dirty = (struct DishInfo *)dirty_dish;
    int i = 0;
    int fd = open(fifo2, O_WRONLY);
    while (1) {
        funcGivePerm((*dirty).semid, -1, 0);
        if ((*dirty).dish_numb1 == i) {
            printf("No dishes left for cleaning\n");
            close(fd);
            return;
        }
        sleep((*dirty).dish_time[i]);
        write(fd, (*dirty).dish_type[i], strlen((*dirty).dish_type[i]));
        write(fd, "\n", 1);
        printf("cleaned %s\n",(*dirty).dish_type[i]);
        funcGivePerm((*dirty).semid, 1, 1);
        
        i++;
    }
}

void *DryingThread(void* wet_dish) {
    struct DishInfo *wet = (struct DishInfo *)wet_dish;
    int i = 0;
    int j,k;
    char buf[50];
    FILE *fp = fopen(fifo2, "r");
    //int fd = open(fifo2, O_RDONLY);
    while (1) {
        funcGivePerm((*wet).semid, -1, 1);
        fgets(buf, 50, fp);
        k = 0;
        for (j = 0; j < (*wet).dish_numb1; j++) {                   //|
            if (strncmp((*wet).dish_type[j] ,buf, strlen(buf)-2) == 0) {            //| to do fix here
                k = j;
                j = (*wet).dish_numb1;
            } 
        }
        
        sleep((*wet).dish_time[k]);
        printf("dryed %s\n",(*wet).dish_type[k]);
        funcGivePerm((*wet).semid, 1, 0);
        i++;
        if (i == (*wet).dish_numb2) {
            printf("No dishes left for drying\n");
            fclose(fp);
            return;
        }
    }

}

int getDishInfo(char *source, int **dish_time, char ***dish_type) {
    FILE* fp = fopen(source, "r");
    char buf1[50];
    long int numb;
    int space = ' ';
    int i = 0;
    while (fgets(buf1, 50, fp) != NULL) {  
        *dish_time = (int *) realloc(*dish_time, (i + 1) * sizeof(int));
        *dish_type = (char **) realloc(*dish_type, (i + 1) * sizeof(char*));
        numb = strchr(buf1, space) - buf1 - 1;
        *(*dish_type + i) = (char *) malloc(numb * sizeof(char));
        strncpy(*(*dish_type + i),buf1, numb);
        *(*dish_time + i) = atoi(strchr(buf1, space));
        *(*(*dish_type + i) + numb) = '\0';
        i++; 
    }
    fclose(fp);
    return i;
}

int main(int argc, int argv[]) {
    char *dirty_dish_source = "dirty_dish.txt";
    char *wet_dish_source = "wet_dish.txt";
    struct DishInfo dirty,wet;
    int i, table_limit, semid;
    dirty.dish_time = (int *) malloc((1) * sizeof(int));
    wet.dish_time = (int *) malloc((1) * sizeof(int));
    dirty.dish_type = (char **) malloc((1) * sizeof(char *));
    wet.dish_type = (char **) malloc((1) * sizeof(char *));

    scanf("%d", &table_limit);
    dirty.dish_numb1 = getDishInfo(dirty_dish_source, &dirty.dish_time, &dirty.dish_type);
    wet.dish_numb1 = getDishInfo(wet_dish_source, &wet.dish_time, &wet.dish_type);
    wet.dish_numb2 = dirty.dish_numb1;
    pthread_t thid1, thid2;
    key_t key; 
    
    if((key = ftok(fifo1,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if((semid = semget(key, 2, 0666 | IPC_CREAT | IPC_EXCL)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }
    dirty.semid = semid;
    wet.semid = semid;
    funcGivePerm(semid, table_limit, 0);
	int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, CleaningThread, &dirty.dish_time);
    funcGivePerm(semid, 0, 1);
    int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, DryingThread, &wet.dish_time);
    
    pthread_join(thid1, (void **)NULL);
    pthread_join(thid2, (void **)NULL);
    semctl(semid, 0, IPC_RMID);
    free(dirty.dish_time);
    free(wet.dish_time);
    for (i = 0; i < dirty.dish_numb1; i++) {
        free(dirty.dish_type[i]);
    }
    free(dirty.dish_type);
    for (i = 0; i < wet.dish_numb1; i++) {
        free(wet.dish_type[i]);
    }
    free(wet.dish_type);
    
    return 0;
}