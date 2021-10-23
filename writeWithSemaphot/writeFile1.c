#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

#define H 1

void funcGivePerm(int id, int token) {
    struct sembuf mybuf; 
    mybuf.sem_op = token;
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if(semop(id, &mybuf, 1) < 0){
        printf("Can\'t wait for condition\n");
        exit(-1);
    }
}

void *writethread1(void* S_fifo) {
    char* buf = "rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrr";
    int fd = open("3.txt", O_WRONLY);
    int i = 0;
    while (i < H) {
        funcGivePerm(fd,-1);
        write(fd, buf, strlen(buf));
        funcGivePerm(fd,1);
        i++;
    }
    close(fd);
}

void *writethread2(void* S_fifo) {
    char* buf = "''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''";
    int fd = open("3.txt", O_WRONLY);
    int i = 0;
    while (i < H) {
        funcGivePerm(fd,-1);
        write(fd, buf, strlen(buf));
        funcGivePerm(fd,1);
        i++;
    }
    close(fd);
}

void *writethread3(void* S_fifo) {
    char* buf = "Some times you need only thirty symbols for error to occur and this thing dont want to work bulls shit";
    int fd = open("3.txt", O_WRONLY);
    int i = 0;
    while (i < H) {
        funcGivePerm(fd,-1);
        write(fd, buf, strlen(buf));
        funcGivePerm(fd,1);
        i++;
    }
    close(fd);
}

int main() {
    char* S_fifo = "3.txt";
    char buf[10000];
    pthread_t thid1, thid2, thid3;

    int semid; 
    char pathname[] = "5.txt"; 
    key_t key; 
    struct sembuf mybuf; 

    if((key = ftok(pathname,0)) < 0){
        printf("Can\'t generate key\n");
        exit(-1);
    }
    if((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0){
        printf("Can\'t get semid\n");
        exit(-1);
    }
    funcGivePerm(semid,1);

    int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, writethread1, S_fifo);
    int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, writethread2, S_fifo);
    int result3 = pthread_create(&thid2, (pthread_attr_t *)NULL, writethread3, S_fifo);
    pthread_join(thid1, (void **)NULL);
    pthread_join(thid2, (void **)NULL);
    pthread_join(thid3, (void **)NULL);
    int fd = open("3.txt", O_RDONLY);
    read(fd, buf, 10000);
    printf("%s\n", buf);
    close(fd);
    return 0;
}
