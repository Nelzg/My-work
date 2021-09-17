#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void StringSplit(char *string, char *delimiters, char **token, int *tokensCount) {
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
}
