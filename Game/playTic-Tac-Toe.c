#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

char turn[] = "Your turn!\n";
char loose[] = "You loose!\n";
char win[] = "You win!\n";
char stalemate[] = "Stalemate!\n";
char wait[] = "Waiting for player\n";
char next[] = "Next turn\n";
char connect1[] = "Connected, you are 'X'";
char connect2[] = "Connected, you are 'O'";

int CheckIfWin(char *a, char p) {
    int i;
    int j;
    int count;
    count = 0;
    for (i = 0; i < 3; i++) {
        if (((a[3 * i] == a[3 * i + 1])&&(a[3 * i + 1] == a[3 * i + 2])&&(a[3 * i + 1] == p))||((a[i] == a[3 + i])&&(a[3 + i] == a[6 + i])&&(a[3 + i] == p))) {
            return 1;
        }
    }
    if (((a[0] == a[4])&&(a[4] == a[8])&&(a[4] == p))||((a[2] == a[4])&&(a[4] == a[6])&&(a[4] == p))) {
        return 1;
    }
    for(i = 0; i < 3; i++) {
        for(j = 0; j < 3; j++) {
            if (a[3 * i + j] != ' ')
                count++;
        } 
    }
    if (count == 9) {
        return 2;
    }
    return 0;
}

void ReadyFrame(char *a, char *b) {
    int i,j;
    for (i = 0; i < 3; i++) {
            strcat(b, "\n+---+---+---+\n");
            for (j = 0; j < 3; j++) {
                strcat(b, "| ");
                b[strlen(b)] = a[3 * i + j];
                strcat(b, " ");
            }
            strcat(b,"|");
        }
        strcat(b,"\n+---+---+---+\n");
}

int MakeYourMove(char *a, char p, int x, int y) {
    
        if ((x > 2)||(x < 0)||(y > 2)||(y < 0)) {
            printf("Wrong move, try again\n");
        }
        
        if (a[3 * x + y] == ' ') {
            a[3 * x + y] = p;
           return 0;
        }
        else {
            printf("The spot is already taken, try again\n");
        }
        
    
    return 0;
}

void Game(char *b, char *a,char *line, int sockfd, int clilen1, int clilen2, struct sockaddr_in cliaddr1,struct sockaddr_in cliaddr2, char player1, char player2)   {
        bzero(b, 180);
        ReadyFrame(a, b);
        sendto(sockfd, b, strlen(b), 0, (struct sockaddr *) &cliaddr1, clilen1);
        sendto(sockfd, b, strlen(b), 0, (struct sockaddr *) &cliaddr2, clilen2);
        if(CheckIfWin(a, player2) == 1) { 
            sendto(sockfd, loose, strlen(loose), 0, (struct sockaddr *) &cliaddr1, clilen1);
            sendto(sockfd, win, strlen(win), 0, (struct sockaddr *) &cliaddr2, clilen2);
            
        }
        else {
            if(CheckIfWin(a, player2) == 2) {
                sendto(sockfd, stalemate, strlen(stalemate), 0, (struct sockaddr *) &cliaddr1, clilen1);
                sendto(sockfd, stalemate, strlen(stalemate), 0, (struct sockaddr *) &cliaddr2, clilen2);
                
            }
            else {
                sendto(sockfd, next, strlen(next), 0, (struct sockaddr *) &cliaddr1, clilen1);
                sendto(sockfd, next, strlen(next), 0, (struct sockaddr *) &cliaddr2, clilen2);
            }
        }
        sendto(sockfd, turn, strlen(turn), 0, (struct sockaddr *) &cliaddr1, clilen1);
        sendto(sockfd, wait, strlen(wait), 0, (struct sockaddr *) &cliaddr2, clilen2);
        recvfrom(sockfd, line, 5, 0, (struct sockaddr *) &cliaddr1, &clilen1);
        printf("%d %d", line[0]-'0',line[2]-'0');
        MakeYourMove(a, player1,line[0]-'0',line[2]-'0');
}


int main() {
    int i;
    char a[9],b[180]="";
    int n = 1;
    for (i = 0; i < 9; i++) {
        a[i] = ' ';
    }
    int sockfd; 
    int clilen1,clilen2;
    char line[5]; 
    struct sockaddr_in servaddr, cliaddr1,cliaddr2; 
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(51000);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if((sockfd = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror(NULL);
        exit(1);
    }
    
    if(bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    
    clilen1 = sizeof(cliaddr1); 
    clilen2 = sizeof(cliaddr2);
    
    if((n = recvfrom(sockfd, line, 5, 0, (struct sockaddr *) &cliaddr1, &clilen1)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    sendto(sockfd, connect1, strlen(connect1), 0, (struct sockaddr *) &cliaddr1, clilen1);
    printf("Connected\n");
        
    if((n = recvfrom(sockfd, line, 5, 0, (struct sockaddr *) &cliaddr2, &clilen2)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    sendto(sockfd, connect2, strlen(connect2), 0, (struct sockaddr *) &cliaddr2, clilen2);
    printf("Connected\n");
    while(1) {
        Game(b,a,line,sockfd,clilen1,clilen2,cliaddr1,cliaddr2,'X','O');
        Game(b,a,line,sockfd,clilen2,clilen1,cliaddr2,cliaddr1,'O','X');
    }
    return 0;
}
