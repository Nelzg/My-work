#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void NForks(int N, int i) {
	int status;
	if (N >= i) {
		pid_t pid = fork();
		// лишние скобки: достаточно писать if (pid == 0 && i < N)
		if ((pid == 0)&&(i < N)) {
			i++;
			printf("Child %d has been born With father %d\n", getpid(),getppid());
			// ставьте пробел после запятых: NForks(N, i); Можно было бы написать NForks(N, i + 1) просто.
			NForks(N,i);
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
	// называйте переменные в одном стиле, т.е. например все со строчной буквы: int n, i;	
	int N,i;
	
	int main_pid = getpid();
	scanf("%d", &N);
	i = 1;
	NForks(N-1,i);
	return 0;
}
