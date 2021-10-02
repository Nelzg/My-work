#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/types.h> 
#include <sys/stat.h>

void conversationBetweenConsoles(char* S_1fifo, char* S_2fifo, char *buf) {
		int fd_fifo;
		pid_t pid = fork();
                if (pid > 0) {
                        fd_fifo = open(S_1fifo,O_WRONLY);
                        while (1) {
                                gets(buf);
                                write(fd_fifo, buf, 100);
                        }
                }
                else {
                        fd_fifo = open(S_2fifo, O_RDONLY);
                        while (1) {
                                read(fd_fifo, buf, 100);
                                printf("%s\n", buf);
                        }
                }
}

int main() {
	int i = 0;
	int c;
	char buf[100];
	char* S_1fifo = "FIFO1.txt";
	char* S_2fifo = "FIFO2.txt";
	scanf("%d", &c);
	mknod(S_1fifo, S_IFIFO | 0666, 0);
	mknod(S_2fifo, S_IFIFO | 0666, 0);
	if (c == 1) {
		conversationBetweenConsoles(S_1fifo, S_2fifo, buf);
	}
	else {
		if (c == 2) {
        		conversationBetweenConsoles(S_2fifo, S_1fifo, buf);
		}
		else {
			printf("Error");
		}
	}	
	return 0;
}
