#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void StringSplit(char *string, char *delimiters, char* **token, int *tokensCount) {
	int i = 0;
	char *saveToken = string;
	int size = 3;
	while (*(*token + i) = strtok_r(saveToken, delimiters, &saveToken)) {
		if (i == (size - 1)) {
			size = size + 3;
			*token = (char **) realloc (*token, size * sizeof(char *));		
                }
		i++;
		(*tokensCount)++;
	}
}

void getString(char** string) {
	int i = 0;
	int size = 3;
        while ((*(*string + i) = getchar()) != '\n' ) {
                if (i == (size - 1)) {
                        size = size + 3;
                        *string = (char *) realloc (*string, size * sizeof(char));
                }
                i++;
        }
        *(*string + i) = '\0';
}

int main() {
	char *string;
	char *delimiters;
	char **token;
	int tokensCount;
	int i,n,size;
	size = 3;	
	tokensCount = 0;
	//I thought of tokensCount as array of sizes of each token

	string = (char *) malloc (size * sizeof(char));
	token = (char **) malloc (size * sizeof(char *));
	delimiters = (char *) malloc (size * sizeof(char));

	getString(&string);
	getString(&delimiters);
	
	StringSplit(string, delimiters, &token, &tokensCount);

	printf("%d\n", tokensCount);
	for (i = 0; i < tokensCount; i++) {
		printf("%s\n", token[i]);
	}

	free(string);
	free(delimiters);
	free(token);
	return 0;
}
