#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void StringSplit(char *string, char *delimiters, char **token, int *tokensCount) {
	int i = 0;
	char *saveToken;
	while (strtok_r(string, delimiters, saveToken) != NULL) {	
		token = *savetoken;
	}

}

int main() {
	char *string;
	char *delimiters;
	char **token;
	int i = 0;
	string = (char *) malloc (3 * sizeof(char));
	*token = (char **) malloc (3 * sizeof(char *));
	while ( (incoming[i]= getchar()) != '\n' ) {
		if ((i % 3) == 2) {
			string = (char *) realloc (string, (((i / 3) + 1) * 3 + 1) * sizeof(char));
		}
		i++;
	}
	strig[i + 1] = '\0';
	return 0;
}
