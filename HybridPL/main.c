
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define lineSize 512


int main() {
	#pragma warning(disable : 4996)
	const char* word;//
	char buffer[lineSize];
	const char* delims = " ";
	char **split = (char**) malloc(50 * sizeof(char*));
	int i = 0;
	int count = 0;
	printf("example:\nvectorsum(1 2 3 4 5 9 7 3 6 8)\npicalculation(10)\n\n");
	gets(buffer, lineSize);

	printf("%s\n", buffer);

	word = strtok(buffer, delims);

	while (word != NULL) {
		//printf("\nword: %s\n", word);
		split[i] = word;
		word = strtok(NULL, delims);
		i++;
		count++;
	}
	i++;
	split[i] = '\0';
	i = 0;
	while (i<count) {
		printf("\n%s\n", split[i++]);			
	}
	free(split);
	printf("count = %d", count);
	
	
	getchar();
	return 0;
}

