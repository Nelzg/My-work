#include <stdio.h>
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
	char** Name;
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
		A[i].Parts = (char **) malloc (A[i].Numb * sizeof(char *));
		for (j = 0; j < A[i].Numb - 1; j++) {
			A[i].Parts[j] = token[n + 1 + j];
		}
		A[i].Parts[A[i].Numb] = "\0";
		*A[i].Name = token[n + A[i].Numb];
		A[i].Time = atoi(token[n + 1 + atoi(token[n])]);
		n = n + A[i].Numb + 2;
	}
	
	clock_t clock_main = clock();
	for (i = 0; i < NumbCommands; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			execvp(*A[i].Name, A[i].Parts);
			printf("wakanda"); 
			exit(0);
		}
		else {
				
		}
	}
	return 0;
}
