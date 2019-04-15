
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#define lineSize 512

int main() {
	#pragma warning(disable : 4996)
	const char* word;
	char buffer[lineSize];
	const char* delims = " ";

	printf("Example:\nVectorSum(1 2 3 4 5 9 7 3 6 8)\nPiCalculation(10)\n\n");
	gets(buffer, lineSize);

	printf("%s\n", buffer);

	word = strtok(buffer, delims);

	while (word != NULL) {
		printf("\nword: %s\n", word);
		word = strtok(NULL, delims);
	}

	printf("\n%s\n", buffer);
	getchar();
	return 0;
}

