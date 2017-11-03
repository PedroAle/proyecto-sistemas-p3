#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include "list.h"
#include "work.h"

#define ARGS_GUIDE "Command: ./primos entrada.txt [-t | -p] [-n N]\n"
#define INVALID_ARGS_MSG "./primos: First argument reserved for the input text file\n"
#define RESULTS_FOLDER "Resultados/"

int validate_params(char* archivo_entrada, int t_flag, int p_flag, int N);
int file_exists(char* file_name);
LIST* exportNumbers(char* file_name);
void dividirTrabajo(LIST* lista_Numeros, int numeroDeTrabajadores, Work* work_pool[]);
void* doWork(void* work);
void liberarWorkPool(Work* work_pool[], int array_length);
int isPrimo(int n);
void output(LIST *head, int n);

int main(int argc, char *argv[]) {
	printf("Proyecto Sistemas de Operacion - Problema 1\n");
	/*
	Estos flags permiten determinar un solo modo (-t o -p) en caso de que se llame -tp | -pt | - t - p | etc,
  que seria el primero en ser llamado
	*/
	int t_flag = 0, p_flag = 0;
	int numeroDeTrabajadores = 0;
	int i;
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
				if (numeroDeTrabajadores > 10 || numeroDeTrabajadores < 1) {
					printf("./primos: 1 <= N <= 10\n");
					return 1;
				}
				break;
		}
	}

	if (!validate_params(archivo_entrada, t_flag, p_flag, numeroDeTrabajadores))
		return 1;

	LIST* lista_numeros = exportNumbers(archivo_entrada);
	//Aqui guardamos las sublistas que le corresponde a cada thread/proceso
	Work* work_pool[numeroDeTrabajadores];
	dividirTrabajo(lista_numeros, numeroDeTrabajadores, work_pool);

	int creador = -1;
	if (t_flag) {
		pthread_t thread[numeroDeTrabajadores];
		for (int i=0; i< numeroDeTrabajadores; i++) {
			pthread_create(&thread[i],NULL, doWork, work_pool[i]);
		}

		for (int i=0; i< numeroDeTrabajadores; i++) {
			pthread_join(thread[i], NULL);
		}
	} else if(p_flag) {
		int pid;
		//Creamos todos los procesos
		for(i=0;i<numeroDeTrabajadores;i++){
				pid = fork();
				if(pid < 0)
				{
						 perror("Fork error\n");
						 exit(1);
				}
				else if (pid==0) //Si es el proceso hijo, realiza su trabajo y luego sale del ciclo para que no cree mas hijos
				{
						doWork(work_pool[i]);
						break;
				} else {
					creador = 1;
				}
		}
	}

	if (creador) {
		wait(NULL);
	}

	liberarLista(lista_numeros);
	liberarWorkPool(work_pool, numeroDeTrabajadores);
	return 0;
}

/**
 * Procesa la lista de numeros de un trabajo guardando el resultado en un nuevo archivo de texto
 * cuyo nombre es igual al del id del work.
 * Es llamada por cada thread/proceso con su Work
 * @param  work   Estructura que almacena la lista de numeros a procesar y el identificador del thread/proceso
 */
void* doWork(void* work) {
	LIST* numbers = ((Work*) work)->toProcess;
	printf("Trabajador %d - Comenzado\n", ((Work*) work)->id);

	LIST* primosHead = NULL, *primosAux = NULL;
	LIST* aux = numbers;
	while (aux) {
		if (isPrimo(aux->n)) {
			//Creamos la primera casilla de la lista que lamacenara los numeros primos
			if (!primosHead) {
				primosHead = crearNodo(aux->n, NULL);
				primosAux = primosHead;
			} else {
				//Si ya esta creada la lista de primos, vamos agregando los nuevos elementos
				LIST* nCasilla = crearNodo(aux->n,NULL);
				primosAux->next = nCasilla;
				primosAux = nCasilla;
			}
		}
		aux = aux->next;
	}
	//Guardamos el resultado en un archivo de texto
	output(primosHead, ((Work*) work)->id);
	liberarLista(primosHead);
	printf("Trabajador %d - Completado\n", ((Work*) work)->id);
}

/**
 * Divide una lista de numeros equitativamente entre una determinada cantidad de trabajadores.
 * @param  lista_numeros        Cabeza de la lista que sera dividida.
 * @param  numeroDeTrabajadores Cantidad de trabajadores que recibiran parte de la lista_numeros
 * @param  work_pool            Variable donde se almacenara el resultado de la division
 */
void dividirTrabajo(LIST* lista_numeros, int numeroDeTrabajadores, Work* work_pool[]) {
	int cantidadDeNumeros = numeroDeElementos(lista_numeros);
	int numerosPorTrabajador = cantidadDeNumeros/numeroDeTrabajadores;
  int numeroUltimoTrabajador = numerosPorTrabajador + cantidadDeNumeros % numeroDeTrabajadores;
  //printf("Numeros P/T: %d\nUltimo Trabajador: %d\nTotal: %d\n", numerosPorTrabajador, numeroUltimoTrabajador,
  //numeroDeTrabajadores-1)*numerosPorTrabajador+numeroUltimoTrabajador);
  int lIndex = 0;
  for (int i=0; i< numeroDeTrabajadores - 1; i++) {
    LIST* numbers = subLista(lista_numeros, lIndex, lIndex+numerosPorTrabajador-1);
		work_pool[i] = crearTrabajo(numbers, i);
    lIndex += numerosPorTrabajador;
  }
    work_pool[numeroDeTrabajadores-1] = crearTrabajo(subLista(lista_numeros,lIndex, lIndex+numeroUltimoTrabajador-1), numeroDeTrabajadores-1);
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

/**
 * Imprime la lista que recibe en un nuevo archivo de texto cuyo nombre viene dado por el parametro n.
 * @method output
 * @param  head   Cabeza de la lista a imprimir.
 * @param  n      Numero que sera el nombre del archivo.
 */
void output(LIST *head, int n){
		struct stat st = {0};

    LIST *current;
    current = head;
		char str[80];

    FILE *fptr;

		if (stat(RESULTS_FOLDER, &st )== -1){
			mkdir(RESULTS_FOLDER,0700);
		}

		sprintf(str, "%s%d.txt", RESULTS_FOLDER, n);
    fptr = fopen(str, "w");

    while (current){
			fprintf(fptr, "%d\n", current->n);
			current = current->next;
    }

    fclose(fptr);
}

/**
 * Libera la memoria ocupada por un arreglo de tipo Work*
 * @param  work_pool       Arreglo de Work* a ser liberado
 * @param  array_length    Longitud del arreglo
 */
void liberarWorkPool(Work* work_pool[], int array_length) {
	for (int i=0; i < array_length; i++) {
		liberarTrabajo(work_pool[i]);
	}
}

/**
 * Determina si un numero es primo o no
 * @param  n       Numero a ser definido como primo o no.
 * @return         1 / 0  = Primo / No primo
 */
int isPrimo(int n){
	if (n == 1)
		return 1;

	if (n == 0)
		return 0;

	//Siempre sera divisible entre el 1
	int divisores = 1;

	for (int i = 2; i <= n; i++) {
		if (n % i == 0)
			divisores++;

		if (divisores > 2)
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
