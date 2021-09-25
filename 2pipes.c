#include <unistd.h>
#include <stdio.h>

int main() {
	int a = 1;
	int b = 1;
	int fd[2],fd1[2];
	int res;
        pipe(fd);
	pipe(fd1);
	res = fork();
	if (res > 0) {
		close(fd[0]);
		close(fd1[1]);
        	write(fd[1],&a,sizeof(a));
		write(fd[1],&b,sizeof(b));
		read(fd1[0],&a,sizeof(a));
		printf("%d\n",a);
	}
	else {
		close(fd1[0]);
                close(fd[1]);
		read(fd[0],&a,sizeof(a));
		read(fd[0],&b,sizeof(b));
		a = a + b;
		write(fd1[1],&a,sizeof(a));
	}
	return 0;
}
