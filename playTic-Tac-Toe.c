#include <stdio.h>

int CheckIfWin(char *a, char p) {
    int i;
    for (i = 0; i < 3; i++) {
            if (((a[3 * i] == a[3 * i + 1])&&(a[3 * i + 1] == a[3 * i + 2])&&(a[3 * i + 1] == p))||((a[i] == a[3 + i])&&(a[3 + i] == a[6 + i])&&(a[3 + i] == p))) {
                printf("Player %c wins\n", p);
                return 1;
            }
        }
    if (((a[0] == a[4])&&(a[4] == a[8])&&(a[4] == p))||((a[2] == a[4])&&(a[4] == a[6])&&(a[4] == p))) {
        printf("Player %c wins\n", p);
        return 1;
    }
    return 0;
}

void PrintFrame(char *a) {
    int i,j;
    for (i = 0; i < 3; i++) {
            printf("\n+---+---+---+\n");
            for (j = 0; j < 3; j++) {
                printf("| %c ", a[3 * i + j]);
            }
            printf("|");
        }
        printf("\n+---+---+---+\n");
}

int MakeYourMove(char *a, char p) {
    int x,y;
    while(1) {
        scanf("%d %d", &x, &y);
        if (a[3 * x + y] == ' ') {
            a[3 * x + y] = p;
            return 0;
        }
        else {
            printf("The spot is already taken, try again\n");
        }
    }
    return 0;
}

int main() {
    int i;
    char a[9];
    int n = 1;
    for (i = 0; i < 9; i++) {
        a[i] = ' ';
    }
    while(1) {
        PrintFrame(a);
        if (CheckIfWin(a,'O')) {
            return 0;
        }
        MakeYourMove(a,'X');
        PrintFrame(a);
        if (CheckIfWin(a,'X')) {
            return 0;
        }
        MakeYourMove(a,'O');
    }
    return 0;
}