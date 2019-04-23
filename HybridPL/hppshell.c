#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define BUFFERSIZE 1024
#pragma warning(disable : 4996)//No borrar
const char* delims = " ,()";
int threadNum;
struct list {
	int size;
	double *listelements;
};

//Definir funciones de los differentes commands.

int vectorsum(char **funct);
int vectorsub(char **funct);
int vectormult(char **funct);
int vectordiv(char **funct);
int setThreadNum(char **funct);
int help();

//Arreglo para guardar el nombre de los diferentes commands currently supported.
const char *functs[] = {
  "vectorsum","vectorsub","vectormult","vectordiv","help","setthreadnum"
};

//Arreglo para guardar los addresses de las funciones de los diferentes commands currently supported.
int(*cmd_functs[]) (char **) = {
  &vectorsum, &vectorsub , &vectormult , &vectordiv, &help, &setThreadNum
};

/**
 * RETURN Cantidad de commands currently supported.
*/
int functcount() { return sizeof(functs) / sizeof(char *); }

//Funciones para implementar el shell.

char * readline();
char ** linetok(char *funct);
int dofunct(char **funct);
struct list makelist(char **funct, int startindex);

int main(int argc, char *argv[]) {
	char **line;
	line = (char**)malloc(sizeof(char)*BUFFERSIZE*BUFFERSIZE);
	int index = 0;

	while (1) {
		printf("HPPShell>>> ");
		line[index] = readline();
		//El command exit cierra el shell
		if (strcmp(line[index], "exit") == 0) {
			printf("Programa terminado.\n");
			exit(0);
		}
		else {
			char **command;
			command = (char**)malloc(sizeof(char)*BUFFERSIZE*BUFFERSIZE);
			command = linetok(line[index]);
      int execute = dofunct(command);
			if (execute == 0) {
				//printf("SUCCESS\n");
			} else if(execute == 1){
        printf("ERROR: Listas de diferentes dimensiones/sizes.\n");
      } else if(execute == 2) {
				printf("ERROR: Command no encontrado o not currently supported.\n");
			}
		}
	}
}

//------------------------lexer----------------------------------
/**
 * Se encarga de leer la linea introducida, caracter por caracter.
 * RETURN El string introducido por el usuario.
*/
char * readline() {
  double c;
	int pos = 0, bufSize = BUFFERSIZE;
	char *read = (char*)malloc(sizeof(char)*bufSize);
	while (1) {
		c = (double) getchar();
		if (c == (double) EOF || c == (double) '\n') {
			read[pos] = (double) '\0';
			return read;
		}
		else {
			read[pos] = c;
		}
		pos++;

		if (pos >= bufSize) {
			bufSize += BUFFERSIZE;
			read = (char*)realloc(read, bufSize);
		}
	}
}
/**
 * Se encarga de dividir en tokens el string de caracteres pasado como argumento.
 * RETURN Arreglo de strings, en el que cada celda del arreglo es un token.
*/
char ** linetok(char *funct) {
	char *token;
	char **tokens = (char**)malloc(sizeof(char*)*BUFFERSIZE);
	int i = 0, counttok = 0;
	token = strtok(funct, delims);
	while (token != NULL) {
		tokens[i] = token;
		token = strtok(NULL, delims);
		i++;
		counttok++;
	}
	tokens[counttok] = "?"; //END OF TOKENS
	return tokens;
}
//-----------------------------------------------------------------

/**
 * Busca en el arreglo de commands currently supported. Si encuentra el command,
 * ejecuta la funcion correspondiente.
 * RETURN Resultado de la ejecucion de la funcion (status).
*/
int dofunct(char **funct) {
	char *cmd;
	cmd = funct[0];

	for (int i = 0; i < functcount(); i++) {
		if (strcmp(cmd, functs[i]) == 0) {
			return (*cmd_functs[i])(funct);
		}
	}
	//Si termina sin return, command no esta currently supported.
	return 2;
}

/**
 * Crea una lista de los elementos introducidos por el usario.
 * RETURN Estructura de lista para ejecutar commands.
*/
struct list makelist(char **funct, int startindex) {
	struct list ltr;
	ltr.listelements = malloc(sizeof(int)*BUFFERSIZE);
	//Comenzar asignando elementos correspondientes a la lista
	double cursor;
  int listindex = 0;
	cursor = strtod(strtok(funct[startindex], delims), NULL);
	while (cursor != strtod("?", NULL) || cursor != strtod("|", NULL)) {
		ltr.listelements[listindex] = cursor;
		listindex++;
		startindex++;
		cursor = strtod(strtok(funct[startindex], delims), NULL);
	}
	ltr.size = listindex;
	return ltr;
}
/**
 * Establece la cantidad de threads a usarse para la ejecucion de los commands en paralelo.
*/
int setThreadNum(char **funct) {
	threadNum = atoi(funct[1]);
	printf("Thread number set to: %d\n", threadNum);
  return 0;
}
/**
 * Imprime en el shell informacion general sobre el manejo del lenguaje.
*/
int help() {
  printf("\n*****************HELP PAGE******************\n");
  printf("Authors: Bernardo Jr. Sein, Coralys Cortes, Manuel Casta�eda, Vincent Prado\n");
  printf("Es un lenguaje funcional para simplificar y facilitar la ejecucion en paralelo de commands.\n");
  printf("Los vectores tienen que tener el mismo size, pero pueden ser de diferentes dimensiones.\n\n");
  printf("Lista de commands currently supported:\n");
  printf("* vectorsum double, double, ... | double, double, ... \tSuma de vectores\n");
  printf("* vectorsub double, double, ... | double, double, ... \tResta de vectores\n");
  printf("* vectormult double, double, ... | double, double, ... \tMultiplicacion de vectores\n");
  printf("* vectordiv double, double, ... | double, double, ... \tDivision de vectores\n");
  printf("\nEjemplo: vectorsum 1 2 3 4 | 1 2 3 4\n");
  return 0;
}

/**
 * Suma los elementos correspondientes de 2 listas en paralelo usando una cantidad de threads determinada.
 * RETURN Status la ejecucion del command.
*/
int vectorsum(char **funct) {
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	int n = list1.size;

	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);

	if (list1.size == list2.size) {
		int chunck = 5;
		printf("Thread count: %d\n", threadNum);
		printf("Chunk: %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
    #pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
      for (i = 0; i < n; i++) {
        result.listelements[i] = list1.listelements[i] + list2.listelements[i];
        //printf("threadID = %d\n", omp_get_thread_num());
      }
		printf("Result:\t");
		for (int i = 0; i < n; i++) {
			printf("%0.2f ", result.listelements[i]);
		}
		printf("\n");
    return 0;
	} else {
		return 1;
	}
}

/**
 * Resta los elementos correspondientes de 2 listas en paralelo usando una cantidad de threads determinada.
 * RETURN Status de la ejecucion del command.
*/
int vectorsub(char **funct) {
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	int n = list1.size;

	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);

	if (list1.size == list2.size) {
		int chunck = 5;
		printf("Thread count: %d\n", threadNum);
		printf("Chunk: %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
    #pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
      for (i = 0; i < n; i++) {
        result.listelements[i] = list1.listelements[i] - list2.listelements[i];
        //printf("threadID = %d\n", omp_get_thread_num());
      }

		printf("Result:\t");
		for (int i = 0; i < list1.size; i++) {
			printf("%0.2f ", result.listelements[i]);
		}
		printf("\n");
    return 0;
	}
	else {
		return 1;
	}
}

/**
 * Multiplica los elementos correspondientes de 2 listas en paralelo usando una cantidad de threads determinada.
 * RETURN Status de la ejecucion del command.
*/
int vectormult(char **funct) {
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	int n = list1.size;

	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);

	if (list1.size == list2.size) {
		int chunck = 5;
		printf("Thread count: %d\n", threadNum);
		printf("Chunk: %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
		#pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
		for (i = 0; i < n; i++) {
			result.listelements[i] = list1.listelements[i] * list2.listelements[i];
			//printf("threadID = %d\n", omp_get_thread_num());
		}

		printf("Result:\t");
		for (int i = 0; i < list1.size; i++) {
			printf("%0.2f ", result.listelements[i]);
		}
		printf("\n");
    return 0;
	}
	else {
		return 1;
	}
}

/**
 * Divide los elementos correspondientes de 2 listas en paralelo usando una cantidad de threads determinada.
 * RETURN Status de la ejecucion del command.
*/
int vectordiv(char **funct) {
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	int n = list1.size;

	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);

	if (list1.size == list2.size) {
		int chunck = 5;
		printf("Thread count: %d\n", threadNum);
		printf("Chunk: %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
		#pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
		for (i = 0; i < n; i++) {
			result.listelements[i] = list1.listelements[i] / list2.listelements[i];
			//printf("threadID = %d\n", omp_get_thread_num());
		}

		printf("Result:\t");
		for (int i = 0; i < list1.size; i++) {
			printf("%0.2f ", result.listelements[i]);
		}
		printf("\n");
    return 0;
	}
	else {
		return 1;
	}
}