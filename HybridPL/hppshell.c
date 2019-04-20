
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define BUFFERSIZE 1024

const char* delims = " ,()";

struct list {
  int size;
  int *listelements;
};

//definir funciones para los commands
int vectorsum(char **funct);
int vectorsub(char **funct);
int vectormult(char **funct);

//guarda los commands permitidos
char *functs[] = {
  "vectorsum","vectorsub","vectormult"
};

//siempre y cuando tengan el mismo indice, apunta a la direccion para ejecutar
//el command dado
int (*cmd_functs[]) (char **) = {
  &vectorsum, &vectorsub , &vectormult
};

//returns cuantos commands son permitidos
int functcount(){ return sizeof(functs)/sizeof(char *); }

//para implementar el Shell/Script
char * readline();
char ** linetok(char *funct);
int dofunct(char **funct);
struct list makelist(char **funct, int startindex);

int main(int argc, char *argv[]){
  //A medida que se introducen commands, se guardaran en <line>
  char **line;
  line = malloc(sizeof(char)*BUFFERSIZE*BUFFERSIZE);
  int index = 0;

  while(1){
    printf("HPPShell>>> ");
    line[index] = readline();
    printf("Entered: %s\n", line[0]);
    //El command exit cierra el shell
    if(strcmp(line[index], "exit") == 0){
      printf("exit success\n");
      exit(0);
    } else {
      //<dofunct()> busca la funcion a ejecutar para el command dado y devuelve
      //un numero diferente a 0 si la encuentra. Otherwise, devuelve 0.
      char **command;
      command = malloc(sizeof(char)*BUFFERSIZE*BUFFERSIZE);
      command = linetok(line[index]);
      if(dofunct(command) != 0){
        printf("succesful execution\n");
      } else {
        printf("ERROR: command not found\n");
      }
    }
  }
}

char * readline(){
  int c, pos = 0, bufSize = BUFFERSIZE;
  char *read = malloc(sizeof(char)*bufSize);
  while(1){
    c = getchar();
    if(c == EOF || c == '\n'){
      read[pos] = '\0';
      return read;
    } else {
      read[pos] = c;
    }
    pos++;

    if(pos >= bufSize){
      bufSize += BUFFERSIZE;
      read = realloc(read, bufSize);
    }
  }
}

char ** linetok(char *funct){
  char *token;
  char **tokens = (char **) malloc(sizeof(char*)*BUFFERSIZE);
  int i = 0, counttok = 0;
  token = strtok(funct, delims);
  while(token != NULL){
    tokens[i] = token;
    token = strtok(NULL, delims);
    i++;
    counttok++;
  }
  tokens[counttok] = " ?"; //END OF TOKENS (como lo de Vincent)
  return tokens;
}

int dofunct(char **funct){
  char *cmd;
  cmd = funct[0];

  for(int i=0; i<functcount(); i++){
    if(strcmp(cmd, functs[i]) == 0){
      return (*cmd_functs[i])(funct);
    }
  }
  //Si termina el loop sin return, significa que no encontro el command en los
  //commands permitidos. Por lo tanto, devuelve 0.
  return 0;
}

struct list makelist(char **funct, int startindex){
  struct list ltr;
  ltr.listelements = malloc(sizeof(int)*BUFFERSIZE);
  //Comenzar asignando elementos correspondientes a la lista
  int cursor, listindex = 0;
  cursor = atoi(strtok(funct[startindex], delims));
  while(cursor != atoi("?") || cursor != atoi("|")){
    ltr.listelements[listindex] = cursor;
    listindex++;
    startindex++;
    cursor = atoi(strtok(funct[startindex], delims));
  }
  ltr.size = listindex;
  return ltr;
}

//funcion para ejecutar vectorsum
//Necesita implementacion
int vectorsum(char **funct){
  printf("vectorsum!\n");
  struct list list1 = makelist(funct, 1);
  struct list list2 = makelist(funct, list1.size + 2);
  printf("first element:%d, list 1 size:%d\n", list1.listelements[0], list1.size);
  printf("first element:%d, list 2 size:%d\n", list2.listelements[0], list2.size);

  //Main code for creating a thread for every element in the first list.
  int result=0;
  omp_set_num_threads(list1.size);
#pragma omp parallel
  {printf("%d \n",omp_get_thread_num());}


  return 1;
}
//Template para sub
int vectorsub(char **funct){
  printf("vectorsub!\n");
  struct list list1 = makelist(funct, 1);
  struct list list2 = makelist(funct, list1.size + 2);
  printf("first element:%d, list 1 size:%d\n", list1.listelements[0], list1.size);
  printf("first element:%d, list 2 size:%d\n", list2.listelements[0], list2.size);

  struct list result;
  result.listelements = malloc(sizeof(int)*BUFFERSIZE);

	#pragma omp parallel for num_threads(list1.size)
	 for (int i = 0; i < list1.size; i++) {
	  result.listelements[i] = list1.listelements[i] - list2.listelements[i];
	  }
	
	 printf("result:\n");
  for (int i=0; i < list1.size; i++) {
	  printf(" %d ", result.listelements[i]);
  }
  printf("\n");
  return 1;
}
//Template para mult
int vectormult(char **funct){
  printf("vectormult!\n");
  struct list list1 = makelist(funct, 1);
  struct list list2 = makelist(funct, list1.size + 2);
  printf("first element:%d, list 1 size:%d\n", list1.listelements[0], list1.size);
  printf("first element:%d, list 2 size:%d\n", list2.listelements[0], list2.size);
  return 1;
}
