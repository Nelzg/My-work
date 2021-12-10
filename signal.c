#include <signal.h>
#include <stdio.h>

void signal_hendler(int nsign) {
    if (nsign == SIGUSR1)
    printf("CHAMPIOOOOOOOOOON\n");
    if (nsign == SIGUSR2)
    printf("RT\n");
}

int main() {
    signal(SIGUSR1, signal_hendler);
    signal(SIGUSR2, signal_hendler);
    pid_t pid = fork();
    if (pid == 0) {
        while(1) {
            kill(getppid(), SIGUSR1);

        }
    }
    else {
        while(1) {
              kill(0, SIGUSR2);
              printf("%d\n", pid);
        }
    }
    return 0;
}