
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define lineSize 512


int main() {
	#pragma warning(disable : 4996)//No borren esto.
	/*---------------------------------------------Lexer-----------------------------------------------------*/
	char buffer[lineSize];//En este buffer es que se escribe el input.
	const char* word;//Cada Token de se guarda aqui y despues se pasa al string de arrays "split" que esta mas abajo.
	const char* delims = " ,()";//Estod son los delimitadores que tengo por ahora.
	char **split = (char**) malloc(50 * sizeof(char*));//Esto es lo mismo que un array de strings.
	int i = 0;//Esta variable es para iterar.
	int countTokens = 0;//Esta variable cuenta el numero de tokens que se encuentran en "split".
	const char * tokens[] = { "VECTORSUM","PIAPROX","RP","LP","COMMA" };//Esto no lo he usado para nada.
	printf("Example:\nVectorSum(1 2 3 4 5 9 7 3 6 8)\npicalculation(10)\n\n");//Esto es un ejemplo del lenguage.

	gets(buffer, lineSize);//Esto lo que hace es que pide un string y lo guarda en el buffer.

	printf("Original string: %s\n", buffer);//printea el string que introdujo el usuario.

	word = strtok(buffer, delims);//strtok divide el string en tokens. Hace una separacion cada vez que encuentra un simbolo de los que esta en "delims".

	while (word != NULL) {//Este loop guarda cada token en un espacio dentro de "split". Repito que "split es un array de strings".
		split[i] = word;
		word = strtok(NULL, delims);
		i++;
		countTokens++;//Para contar los tokens
	}
	
	i = 0;
	while (i< countTokens) {//Esto es para probar que se guardaron bien los tokens.
		printf("\n%s\n", split[i++]);			
	}
	
	printf("Number of tokens = %d\n", countTokens);
	split[countTokens] = "?";//Este signo de interrogacion lo puse al final del array para saber cuando parar. 
	//Creo que se puede implementar de forma que no haga falta, pero lo voy a dejar asi por ahora.
	countTokens++;


	//----------------------------Rules---------------------------------
	/*
	Esto no esta perfecto porque VectorSum tiene que aceptar dos listas de numeros
	y por ahora solo coge una.
	*/
	i = 1;//La variable i = 1 porque ya sabemos que el primer string es "VectorSum". Lo que le sigue tienen que ser numeros.
	int countNumbers = 0;//Amount of numbers stored in array "numbers".
	int numbers[50];
	int equalStrings = strcmp(split[0], "VectorSum");//Si el primer token es VectorSum, guarda cero en la variable.
	if (equalStrings == 0) {
		while (strcmp(split[i],"?") != 0) {
			numbers[i - 1] = atoi(split[i]);//Anade numeros a la lista y los convierte en enteros.
			countNumbers++;//Cuenta cuantos numeros hay.
			i++;
		}
	}
	else
		printf("Error");

	i = 0;
	printf("countNumbers = %d\n", countNumbers);
	printf("List of numbers:\n");
	for (i = 0; i < countNumbers; i++) {//Con esto pruebo si se guardan los numeros en el array "numbers".
		printf("%d\n", numbers[i]);
	}
	
	free(split);//Libera la memoria.
	getchar();
	return 0;
}

 



