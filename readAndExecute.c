#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

//it is not ready but i tried

void StringSplit(char *string, char* **token, int *tokensCount) {
        int i = 0;
	char* delimiter = "\n";
        char *saveToken = string;
        int size = 3;
        while (*(*token + i) = strtok_r(saveToken, delimiter, &saveToken)) {
                if (i == (size - 1)) {
                        size = size + 3;
                        *token = (char **) realloc (*token, size * sizeof(char *));
                }
                i++;
                (*tokensCount)++;
        }
}

void readFile(char** string, int fd) {
	int i = 0;
	int size = 3;
        while (read(fd, (*string + i), 1) != 0) {
                if (i == (size - 1)) {
                        size = size + 3;
                        *string = (char *) realloc (*string, size * sizeof(char));
                }
                i++;
        }
        *(*string + i) = '\0';
}

struct CommandForFile {
	int Numb;
	char** Parts;
	char* Name;
	int Time;
};

int main() {
	int fd;
	char* file = "ProgrammsToRun.txt";
	char *string;
        char **token;
        int tokensCount;
        int i,n,size,NumbCommands,j;
        size = 3;
        tokensCount = 0;
        string = (char *) malloc (size * sizeof(char));
        token = (char **) malloc (size * sizeof(char *));
	
	fd = open(file, O_RDONLY);
	readFile(&string, fd);
	
	StringSplit(string, &token, &tokensCount);
	
	NumbCommands = 0;
	for(i = 0; i < tokensCount; i++) {
		if (atoi(token[i]) != 0) {
			NumbCommands = NumbCommands + 1;
		}
	}
	NumbCommands = NumbCommands/2;

	struct CommandForFile A[NumbCommands];
	n = 0;
	for (i = 0; i < NumbCommands; i++) {
		A[i].Numb = atoi(token[n]);
		A[i].Parts = (char **) malloc ((A[i].Numb + 1) * sizeof(char *));
		for (j = 0; j < A[i].Numb; j++) {
			A[i].Parts[j] = token[n + 1 + j];
		}
		A[i].Parts[A[i].Numb] = NULL;
		A[i].Name = token[n];
		A[i].Time = atoi(token[n + 1 + atoi(token[n])]);
		n = n + A[i].Numb + 2;
	}
	struct CommandForFile buf;

	for (i = 0; i < NumbCommands-1; i++) {
		if (A[i].Time > A[i + 1].Time) {
			buf = A[i];
			A[i] = A[i + 1];
			A[i + 1] = buf;
		}
	}

//	printf("%s 111\n",A[0].Parts[0]);
	clock_t clock_main = clock();
	for (i = 0; i < NumbCommands; i++) {	
	//	sleep(A[i].Time);
		int fd[2];
		pid_t pid = fork();
		pipe(fd);
		clock_t clock_proc;
		if (pid == 0) {
			close(fd[0]);
			clock_proc = clock();
		//	printf("no\n");
			write(fd[1], &clock_proc, 1);
			printf("n\n");
			execvp(A[i].Name, A[i].Parts);
		}
		if (pid > 0) {
			close(fd[1]);
		//	printf("yes\n");
			read(fd[0], &clock_proc, 1);
			if ((clock_proc - clock_main)/CLOCKS_PER_SEC >= 5) {		
				kill(0,1);
		//		printf("killed");
			}
		}
		
	}
	return 0;
}
