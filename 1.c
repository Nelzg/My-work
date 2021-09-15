#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void GenerateString(int n, char* string) {
	int i;
	for (i = 0; i < n; i++) {
		string[(1 << i) - 1] = 'a' + i;
		strncpy(string + (1 << i), string, (((1 << i) - 1)));
	}
}

int main() {
	int n;
	char* string;
	scanf("%d", &n);
	string = (char*) malloc (((1 << (n + 1)) - 1) * sizeof(char));
	GenerateString(n, string);
	printf("%s\n", string);
	free(string);
	return 0;
}
