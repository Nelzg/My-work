#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void NForks(int N, int i) {
	int status;
	if (N >= i) {
		pid_t pid = fork();
		if (pid == 0 && i < N) {
			printf("Child %d has been born With father %d\n", getpid(), getppid());
			NForks(N, i++);
		}
		else {
			if ((pid==0)&&(i == N)) {
				printf("I am the last one - %d\n", getpid());
			}
			else {
				printf("%d - I am waiting My father is %d\n", getpid(), getppid());
				wait(&status);
				printf("Oh no my son died - %d\n", getpid());
			}
		}
	}
	else {
		printf("%d I am the only process\n", getpid());
	}
}

int main() {
	int N, I;
	
	int main_pid = getpid();
	scanf("%d", &N);
	I = 1;
	NForks(N-1, I);
	return 0;
}
