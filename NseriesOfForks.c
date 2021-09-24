#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
	int N,i;
	int* status =NULL;
	int main_pid = getpid();
	scanf("%d", &N);
	i = 1;
	while (i < N) {
		i++;
		pid_t pid = fork();
		if (pid == 0){
			if ((getpid()-main_pid)>N){
				kill(pid,SIGINT);
			}
			printf("Child process %d has been born\n", getpid()-main_pid);
		}
		else {
			wait(status);
			printf("%d:My child died\n", getpid()-main_pid);
		}	
	}
	return 0;
}
