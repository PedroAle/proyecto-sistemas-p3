#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include "list.h"
#define ARGS_GUIDE "Command: ./primos entrada.txt [-t | -p] [-n N]\n"
#define INVALID_ARGS_MSG "./primos: First argument reserved for the input text file\n"

int validate_params(char* archivo_entrada, int t_flag, int p_flag, int N);
int file_exists(char* file_name);
LIST* exportNumbers(char* file_name);
void dividirTrabajo(LIST* lista_Numeros, int numeroDeTrabajadores, LIST* work_pool[]);

int main(int argc, char *argv[]) {
	printf("Proyecto Sistemas de Operacion - Problema 1\n");
	/*
	Estos flags permiten determinar un solo modo (-t o -p) en caso de que se llame -tp | -pt | - t - p | etc,
  que seria el primero en ser llamado
	*/
	int t_flag = 0, p_flag = 0;
	int numeroDeTrabajadores = 0;
	char * archivo_entrada;
	if (argc > 1) {
    //Aumentamos el indice optind para reservar el primer argumento para el archivo de texto de entrada
    optind = 2;
		archivo_entrada = argv[1];
	} else {
    printf("Missing required arguments\n%s\n", ARGS_GUIDE);
    return 1;
  }

	int c;
	while ((c = getopt(argc,argv,"tpn:")) != -1) {
		switch(c) {
			case 't':
				if (!p_flag)
					t_flag = 1;

				break;

			case 'p':
				if (!t_flag)
					p_flag = 1;

				break;

			case 'n':
				numeroDeTrabajadores = atoi(optarg);
				break;
		}
	}

	if (!validate_params(archivo_entrada, t_flag, p_flag, numeroDeTrabajadores))
		return 1;

	LIST* lista_numeros = exportNumbers(archivo_entrada);
	//Aqui guardamos las sublistas que le corresponde a cada thread/proceso
	LIST* workpool[numeroDeTrabajadores];
	dividirTrabajo(lista_numeros, numeroDeTrabajadores, workpool);

	for (int i=0; i<numeroDeTrabajadores; i++) {
		printf("-------  %d  ----------\n", i);
		imprimirLista(workpool[i]);
		limpiarLista(workpool[i]);
	}

	limpiarLista(lista_numeros);
	return 0;
}

void dividirTrabajo(LIST* lista_numeros, int numeroDeTrabajadores, LIST* work_pool[]) {
	int cantidadDeNumeros = numeroDeElementos(lista_numeros);
	int numerosPorTrabajador = cantidadDeNumeros/numeroDeTrabajadores;
  int numeroUltimoTrabajador = numerosPorTrabajador + cantidadDeNumeros % numeroDeTrabajadores;
  //printf("Numeros P/T: %d\nUltimo Trabajador: %d\nTotal: %d\n", numerosPorTrabajador, numeroUltimoTrabajador,
  //numeroDeTrabajadores-1)*numerosPorTrabajador+numeroUltimoTrabajador);
  int lIndex = 0;
  for (int i=0; i< numeroDeTrabajadores - 1; i++) {
    work_pool[i] = subLista(lista_numeros, lIndex, lIndex+numerosPorTrabajador-1);
    lIndex += numerosPorTrabajador;
  }
  work_pool[numeroDeTrabajadores-1] = subLista(lista_numeros,lIndex, lIndex+numeroUltimoTrabajador-1);
}

/**
 * Valida los argumentos necesarios, devuelve 1 si son validos y 0 si no lo son
 * @param   archivo_entrada Archivo de texto que contiene los numeros a trabajar.
 * @param   t_flag          Estado que determina si el programa esta en modo hilos.
 * @param   p_flag 				 Estado que determina si el programa esta en modo procesos.
 * @param   N      				 Numero de procesos/hilos que usara el programa.
 * @return  								 0 - Input invalido. 1 - Input valido.
 */
int validate_params(char* archivo_entrada, int t_flag, int p_flag, int N) {
	//TODO: Validar que el archivo de entrada exista
	if (!file_exists(archivo_entrada)){
		printf("./primos: Invalid input file\n");
		return 0;
	}

	if (t_flag == 0 && p_flag == 0) {
		printf("./primos: -t or -p argument required.\n%s\n%s\n", INVALID_ARGS_MSG, ARGS_GUIDE);
		return 0;
	}

	if (N == 0) {
		printf("./primos: -n N argument required. 1 <= N <= 10\n%s\n%s\n", INVALID_ARGS_MSG, ARGS_GUIDE);
		return 0;
	}

	return 1;
}

/**
 * Funcion que verifica la existencia de un archivo
 * @param  file_name Direccion en la cual se verificara si hay un archivo.
 * @return           0 - No existe el archivo. 1 - Existe el archivo
 */
int file_exists(char* file_name) {
	FILE* file = fopen(file_name, "r");

	if (file){
		fclose(file);
		return 1;
	}
	else
		return 0;
}

/**
 * Exporta los numeros de un archivo de texto (un numero por linea) a una lista
 * @param  file_name  archivo que sera abierto
 * @return            puntero a la cabeza de una nueva lista que contiene los numeros del archivo. Null/0 si no habian numeros
 */
LIST* exportNumbers(char* file_name){
	FILE *fp;
  char line[10];
  LIST *current, *head;

  head = current = NULL;
  fp = fopen(file_name, "r");

  while(fgets(line, sizeof(line), fp)){
      LIST *node = malloc(sizeof(LIST));
      node->n = atoi(line);
      node->next =NULL;

      if(head == NULL){
          current = head = node;
      } else {
          current = current->next = node;
      }
  }
  fclose(fp);

  return head;
}

void output(LIST *head){

    LIST *current;
    current = head;

    FILE *fptr;
    int n, c = 2, op=1;

    /*  open for writing */
    fptr = fopen("primes.txt", "w");

    if (fptr == NULL){
        printf("File does not exists \n");
        return;
    }

    while (current){

			n = current->n;

    for ( c = 2 ; c <= n - 1 ; c++ ){

        if ( n%c == 0 ){
            break;
        }

    }
       if ( c == n )
          fprintf(fptr, "%d\n", current->n);

      current = current->next;

    }
    fclose(fptr);
}
