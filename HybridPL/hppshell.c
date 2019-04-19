#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define lineSize 512
#define bufferSize 1024

const char* delims = " ,()";

//definir funciones para los commands
int vectorsum(char *funct);

//guarda los commands permitidos
char *functs[] = {
  "vectorsum"
};

//siempre y cuando tengan el mismo indice, apunta a la direccion para ejecutar
//el command dado
int (*cmd_functs[]) (char *) = {
  &vectorsum
};

//returns cuantos commands son permitidos
int functcount(){ return sizeof(functs)/sizeof(char *); }

//para implementar el Shell/Script
char* readline();
int dofunct(char *funct);

int main(int argc, char *argv[]){
  //A medida que se introducen commands, se guardaran en <line>
  char **line;
  line = malloc(sizeof(char)*bufferSize*bufferSize);
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
      if(dofunct(line[index]) != 0){
        printf("succesful execution\n");
      } else {
        printf("ERROR: command not found\n");
      }
    }
  }
}

char * readline(){
  int c, pos = 0, bufSize = bufferSize;
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
      bufSize += bufferSize;
      read = realloc(read, bufSize);
    }
  }
}

int dofunct(char *funct){
  char *cmd;
  cmd = strtok(cmd, delims);

  for(int i=0; i<functcount(); i++){
    if(strcmp(cmd, functs[i]) == 0){
      return (*cmd_functs[i])(funct);
    }
  }
  //Si termina el loop sin return, significa que no encontro el command en los
  //commands permitidos. Por lo tanto, devuelve 0.
  return 0;
}

//funcion para ejecutar vectorsum
//Necesita implementacion
int vectorsum(char *funct){
  printf("vectorsum!\n");
  return 1;
}