#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int StringSplit(char *string, char *delimiters, char **token, int *tokensCount) {
	int i = 0;
	char *saveToken = string;
	int size = 3;
	while (token[i] = strtok_r(saveToken, delimiters, &saveToken)) {
		if (i == (size - 1)) {
			size = size + 3;
			tokensCount = (int *) realloc (tokensCount, size * sizeof(int));
			token = (char **) realloc (token, size * sizeof(char *));		
                }
		tokensCount[i] = strlen(token[i]);
		i++;
	}
	//dont forget to free
	return i;
}

int main() {
	char *string;
	char *delimiters;
	char **token;
	int *tokensCount;
	int i,n,size;
	size = 3;	

	string = (char *) malloc (size * sizeof(char));
	token = (char **) malloc (size * sizeof(char *));
	tokensCount = (int *) malloc (size * sizeof(int));
	delimiters = (char *) malloc (size * sizeof(char));

	i = 0;
	while ((string[i] = getchar()) != '\n' ) {
		if (i == (size - 1)) {
			size = size + 3;
			string = (char *) realloc (string, size * sizeof(char));
		}
		i++;
	}
	string[i] = '\0';

	i = 0;
	while ((delimiters[i] = getchar()) != '\n' ) {
                if (i == (size - 1)) {
			size = size + 3;
                        delimiters = (char *) realloc (delimiters, size * sizeof(char));
                }
                i++;
        }
	delimiters[i] = '\0';

	n = StringSplit(string, delimiters, token, tokensCount);
	for (i = 0; i < n; i++) {
		printf("%s , %d\n", token[i], tokensCount[i]);
	}
	//dont forget to free
	return 0;
}
