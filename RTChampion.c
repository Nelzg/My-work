#include <signal.h>
#include <unistd.h>
#include <stdio.h>

pid_t pid;

void signal_hendler(int nsign) {
    if (nsign == SIGUSR2) {
        printf("CHAMPIOOOOOOOOOON\n");
        kill(pid, SIGUSR1);
    }
    if (nsign == SIGUSR1) {
        printf("RT\n");
        kill(0, SIGUSR2);
    }
}

int main() {
    signal(SIGUSR1, signal_hendler);
    signal(SIGUSR2, signal_hendler);
    kill(pid, SIGUSR1);
    pid = fork();
    if (pid == 0) {
        while(1){
        }
    }
    else {
        while(1){
        }
    }
    return 0;
}
