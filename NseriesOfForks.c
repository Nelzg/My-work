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
			printf("Child process %d with father %d\n", getpid(),getppid());
		}
		else {
		//	printf("Father process %d with father %d waits\n", pid,getppid());
			wait(status);
		}	
	}
	return 0;
}
