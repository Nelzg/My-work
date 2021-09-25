#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void NForks(int N, int i) {
	int status;
	pid_t pid = fork();
	if ((pid == 0)&&(i < N)) {
		i++;
		printf("Child %d has been born With father %d\n", getpid(),getppid());
		NForks(N,i);
	}
	else {
		if ((pid==0)&&(i == N)) {
	//		kill(pid,SIGINT);
	//	if ((pid == 0) && (i == N-1)) {
			printf("I am the last one - %d\n", getpid());
		}
		else {
			printf("%d - I am waiting My father is %d\n", getpid(), getppid());
			wait(&status);
			printf("Oh no my son died - %d\n", getpid());
		}
	}
		
}

int main() {
	int N,i;
	
	int main_pid = getpid();
	scanf("%d", &N);
	i = 1;
	NForks(N-1,i);
	return 0;
}
