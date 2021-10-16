#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>

char* S_1fifo = "FIFO1.txt";
char* S_2fifo = "FIFO2.txt";

void *mythread1(void* dummy) {
	int fd_fifo = open(S_1fifo, O_WRONLY);
	char buf[100];
	while (1) {
        	gets(buf);
    		write(fd_fifo, buf, 100);
    	}
}	

void *mythread2(void* dummy) {
	int fd_fifo = open(S_2fifo, O_RDONLY);
	char buf[100];
    	while (1) {
        	read(fd_fifo, buf, 100);
        	printf("%s\n", buf);
    	}
}	

void *mythread3(void* dummy) {
	int fd_fifo = open(S_2fifo, O_WRONLY);
	char buf[100];
	while (1) {
        	gets(buf);
    		write(fd_fifo, buf, 100);
    	}
}	

void *mythread4(void* dummy) {
	int fd_fifo = open(S_1fifo, O_RDONLY);
	char buf[100];
    	while (1) {
        	read(fd_fifo, buf, 100);
        	printf("%s\n", buf);
    	}
}	

void conversationBetweenConsoles1() {
	pthread_t thid1, thid2;
	int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, mythread1, NULL);
	int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, mythread2, NULL);
	pthread_join(thid1, (void **)NULL);
	pthread_join(thid2, (void **)NULL);
}

void conversationBetweenConsoles2() {
	pthread_t thid1, thid2;
	int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, mythread3, NULL);
	int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, mythread4, NULL);
	pthread_join(thid1, (void **)NULL);
	pthread_join(thid2, (void **)NULL);
}

int main() {
	int i = 0;
	int c;
	char buf[100];
	scanf("%d", &c);
	mknod(S_1fifo, S_IFIFO | 0666, 0);
	mknod(S_2fifo, S_IFIFO | 0666, 0);
	if (c == 1) {
		conversationBetweenConsoles1(S_1fifo, S_2fifo, buf);
	}
	else {
		if (c == 2) {
        		conversationBetweenConsoles2(S_2fifo, S_1fifo, buf);
		}
		else {
			printf("Error");
		}
	}	
	return 0;
}
