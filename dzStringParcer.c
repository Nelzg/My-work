#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int StringSplit(char *string, char *delimiters, char **token, int *tokensCount) {
	int i = 0;
	char *saveToken = string;
	while (token[i] = strtok_r(saveToken, delimiters, &saveToken)) {
		if ((i % 3) == 2) {
			tokensCount = (int *) realloc (tokensCount, (((i / 3) + 1) * 3 + 1) * sizeof(int));
                }
		tokensCount[i] = strlen(token[i]);
		i++;
	}
	return i;
}

int main() {
	char *string;
	char *delimiters;
	char **token;
	int *tokensCount;
	int i,n;

	string = (char *) malloc (3 * sizeof(char));
	token = (char **) malloc (3 * sizeof(char *));
	tokensCount = (int *) malloc (3 * sizeof(int));
	delimiters = (char *) malloc (3 * sizeof(char));

	i = 0;
	while ((string[i] = getchar()) != '\n' ) {
		if ((i % 3) == 2) {
			string = (char *) realloc (string, (((i / 3) + 1) * 3 + 1) * sizeof(char));
		}
		i++;
	}
	string[i + 1] = '\0';

	i = 0;
	while ((delimiters[i] = getchar()) != '\n' ) {
                if ((i % 3) == 2) {
                        delimiters = (char *) realloc (delimiters, (((i / 3) + 1) * 3 + 1) * sizeof(char));
                }
                i++;
        }

	n = StringSplit(string, delimiters, token, tokensCount);
	for (i = 0; i < n; i++) {
		printf("%s , %d\n", token[i], tokensCount[i]);
	}

	return 0;
}
