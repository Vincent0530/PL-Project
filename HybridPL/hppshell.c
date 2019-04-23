#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <time.h>

#define BUFFERSIZE 1024
#pragma warning(disable : 4996)//No borren esto.
const char* delims = " ,()";
int threadNum;
struct list {
	int size;
	double *listelements;
};

//definir funciones para los commands
int vectorsum(char **funct);
int vectorsub(char **funct);
int vectormult(char **funct);
int vectordiv(char **funct);
void setThreadNum(char **funct);
void help();

//guarda los commands permitidos
const char *functs[] = {
  "vectorsum","vectorsub","vectormult","vectordiv","help","setthreadnum"
};

//siempre y cuando tengan el mismo indice, apunta a la direccion para ejecutar
//el command dado
int(*cmd_functs[]) (char **) = {
  &vectorsum, &vectorsub , &vectormult , &vectordiv, &help, &setThreadNum
};

//returns cuantos commands son permitidos
int functcount() { return sizeof(functs) / sizeof(char *); }

//para implementar el Shell/Script
char * readline();
char ** linetok(char *funct);
int dofunct(char **funct);
struct list makelist(char **funct, int startindex);

int main(int argc, char *argv[]) {
	//A medida que se introducen commands, se guardaran en <line>
	char **line;
	line = (char**)malloc(sizeof(char)*BUFFERSIZE*BUFFERSIZE);
	int index = 0;

	while (1) {
		printf("HPPShell>>> ");
		line[index] = readline();
		printf("Entered: %s\n", line[0]);
		//El command exit cierra el shell
		if (strcmp(line[index], "exit") == 0) {
			printf("exit success\n");
			exit(0);
		}
		else {
			//<dofunct()> busca la funcion a ejecutar para el command dado y devuelve
			//un numero diferente a 0 si la encuentra. Otherwise, devuelve 0.
			char **command;
			command = (char**)malloc(sizeof(char)*BUFFERSIZE*BUFFERSIZE);
			command = linetok(line[index]);
			if (dofunct(command) != 0) {
				// printf("succesful execution\n");
			}
			else {
				printf("ERROR: command not found\n");
			}
		}
	}
}

//------------------------lexer----------------------------------
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
	tokens[counttok] = "?"; //END OF TOKENS (como lo de Vincent)
	return tokens;
}
//-----------------------------------------------------------------


int dofunct(char **funct) {
	char *cmd;
	cmd = funct[0];

	for (int i = 0; i < functcount(); i++) {
		if (strcmp(cmd, functs[i]) == 0) {
			return (*cmd_functs[i])(funct);
		}
	}
	//Si termina el loop sin return, significa que no encontro el command en los
	//commands permitidos. Por lo tanto, devuelve 0.
	return 0;
}

struct list makelist(char **funct, int startindex) {
	struct list ltr;
	ltr.listelements = malloc(sizeof(int)*BUFFERSIZE);
	//Comenzar asignando elementos correspondientes a la lista
	double cursor;
  int listindex = 0;
	cursor = strtod(strtok(funct[startindex], delims), NULL);
	while (cursor != strtod("?", NULL) || cursor != strtod("|", NULL)) {
    printf("cursor is: %0.2f\n", cursor);
		ltr.listelements[listindex] = cursor;
		listindex++;
		startindex++;
		cursor = strtod(strtok(funct[startindex], delims), NULL);
	}
	ltr.size = listindex;
	return ltr;
}

void setThreadNum(char **funct) {
	threadNum = atoi(funct[1]);
	printf("\nthreadNum = %d\n", threadNum);
}

void help() {
	printf("\n\n *****************HELP PAGE******************\n");
	printf("\n Authors:Bernardo Jr. Sein, Coralys Cortes, Manuel Casta�eda, Vincent Prado \n ");
	printf("\n***** This is a hybrid language which automatically uses multiple threads for each function currently supported*****\n");
	printf("Currently supported functions: \n");
	printf("\n-vectorsum double double | double double       (sumation of vectors) \n"
		"\n-vectorsub double double | double double      (substraction of vectors) \n"
		"\n-vectormult double double | double double     (multiplication of vectors) \n"
    "\n-vectordiv double double | double double      (division of vectors) \n"
		"\n(vectors can have any amount of dimensions, but both vectors must be of equal sizes)\n \n"
		"\n to exit the program type 'exit' \n\n");
	printf("Syntax Example:\n\nvectorsum 1 2 3 4 5 | 6 7 8 9 10\n\n");
}

//funcion para ejecutar vectorsum
//Necesita implementacion
int vectorsum(char **funct) {
	printf("vectorsum!\n");
	struct list list1 = makelist(funct, 1);//El segundo parametro es igual a 1 porque el primera string de la lista es el comando que define la operacion
	struct list list2 = makelist(funct, list1.size + 2);
	printf("first element:%0.2f, list 1 size:%d\n", list1.listelements[0], list1.size);
	printf("first element:%0.2f, list 2 size:%d\n", list2.listelements[0], list2.size);
	int n = list1.size;



	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);

	if (list1.size == list2.size) {
		//clock_t t;
		//t = clock();
		int chunck = 5;
		printf("\nthreadNum = %d\n", threadNum);
		printf("\nchunck = %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
#pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
		for (i = 0; i < n; i++) {
			result.listelements[i] = list1.listelements[i] + list2.listelements[i];
			printf("threadID = %d\n", omp_get_thread_num());
		}


		printf("\nresult: ");
		for (int i = 0; i < n; i++) {
			printf(" %0.2f ", result.listelements[i]);
		}
		printf("\n");
		//t = clock() - t;
		//double time_taken = ((double)t) / CLOCKS_PER_SEC;
	   // printf("Time taken = %f\n", time_taken);

	}
	else {
		printf("\nError: Vectors Must be of equal size. \n");
	}



	return 1;
}
//Template para sub
int vectorsub(char **funct) {
	printf("vectorsub!\n");
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	printf("first element:%0.2f, list 1 size:%d\n", list1.listelements[0], list1.size);
	printf("first element:%0.2f, list 2 size:%d\n", list2.listelements[0], list2.size);
	int n = list1.size;
	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);
	if (list1.size == list2.size) {
		int chunck = 5;
		printf("\nthreadNum = %d\n", threadNum);
		printf("\nchunck = %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
#pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
		for (i = 0; i < n; i++) {
			result.listelements[i] = list1.listelements[i] - list2.listelements[i];
			printf("threadID = %d\n", omp_get_thread_num());
		}

		printf("\nresult: ");
		for (int i = 0; i < list1.size; i++) {
			printf(" %0.2f ", result.listelements[i]);
		}
		printf("\n");
	}
	else {
		printf("\nError: Vectors Must be of equal size. \n");

	}

	return 1;
}
//Template para mult
int vectormult(char **funct) {
	printf("vectormult!\n");
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	printf("first element:%0.2f, list 1 size:%d\n", list1.listelements[0], list1.size);
	printf("first element:%0.2f, list 2 size:%d\n", list2.listelements[0], list2.size);
	int n = list1.size;
	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);
	if (list1.size == list2.size) {
		int chunck = 5;
		printf("\nthreadNum = %d\n", threadNum);
		printf("\nchunck = %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
		#pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
		for (i = 0; i < n; i++) {
			result.listelements[i] = list1.listelements[i] * list2.listelements[i];
			printf("threadID = %d\n", omp_get_thread_num());
		}

		printf("\nresult: ");
		for (int i = 0; i < list1.size; i++) {
			printf(" %0.2f ", result.listelements[i]);
		}
		printf("\n");
	}
	else {
		printf("\nError: Vectors Must be of equal size. \n");

	}

	return 1;
}
//Template para div
int vectordiv(char **funct) {
	printf("vectordiv!\n");
	struct list list1 = makelist(funct, 1);
	struct list list2 = makelist(funct, list1.size + 2);
	printf("first element:%0.2f, list 1 size:%d\n", list1.listelements[0], list1.size);
	printf("first element:%0.2f, list 2 size:%d\n", list2.listelements[0], list2.size);
	int n = list1.size;
	struct list result;
	result.listelements = malloc(sizeof(int)*BUFFERSIZE);
	if (list1.size == list2.size) {
		int chunck = 5;
		printf("\nthreadNum = %d\n", threadNum);
		printf("\nchunck = %d\n", chunck);
		int i;
		omp_set_num_threads(threadNum);
		#pragma omp parallel for shared (list1,list2,result,n) private(i) schedule(static,chunck)
		for (i = 0; i < n; i++) {
			result.listelements[i] = list1.listelements[i] / list2.listelements[i];
			printf("threadID = %d\n", omp_get_thread_num());
		}

		printf("\nresult: ");
		for (int i = 0; i < list1.size; i++) {
			printf(" %0.2f ", result.listelements[i]);
		}
		printf("\n");
	}
	else {
		printf("\nError: Vectors Must be of equal size. \n");

	}

	return 1;
}