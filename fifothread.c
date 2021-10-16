#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <pthread.h>

void *writethread1(void* S_1fifo) {
	int fd_fifo = open(S_1fifo, O_WRONLY);
	char buf[100];
	while (1) {
        	gets(buf);
    		write(fd_fifo, buf, 100);
    	}
}	

void *readthread2(void* S_2fifo) {
	int fd_fifo = open(S_2fifo, O_RDONLY);
	char buf[100];
    	while (1) {
        	read(fd_fifo, buf, 100);
        	printf("%s\n", buf);
    	}
}	

void conversationBetweenConsoles(char* S_1fifo,char*  S_2fifo) {
	pthread_t thid1, thid2;
	int result1 = pthread_create(&thid1, (pthread_attr_t *)NULL, writethread1, S_1fifo);
	int result2 = pthread_create(&thid2, (pthread_attr_t *)NULL, readthread2, S_2fifo);
	pthread_join(thid1, (void **)NULL);
	pthread_join(thid2, (void **)NULL);
}

int main() {
	int c;
	char* S_1fifo = "FIFO1.txt";
	char* S_2fifo = "FIFO2.txt";
	scanf("%d", &c);
	mknod(S_1fifo, S_IFIFO | 0666, 0);
	mknod(S_2fifo, S_IFIFO | 0666, 0);
	if (c == 1) {
		conversationBetweenConsoles(S_1fifo, S_2fifo);
	}
	else {
		if (c == 2) {
        		conversationBetweenConsoles(S_2fifo, S_1fifo);
		}
		else {
			printf("Error");
		}
	}	
	return 0;
}
