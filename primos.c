#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define INPUT_ARG_POS 1
#define ARGS_GUIDE "Command: ./primos entrada.txt [-t | -p] [-n N]\n"
#define INVALID_ARGS_MSG "./primos: First argument reserved for the input text file\n"

struct list {
    int n;
    struct list *next;
};

typedef struct list LIST;

int validate_params(char* archivo_entrada, int t_flag, int p_flag, int N);
int file_exists(char* file_name);
LIST* exportNumbers(char* file_name);

int main(int argc, char *argv[]) {
	printf("Proyecto Sistemas de Operacion - Problema 1\n");
	//Estos flags permiten determinar un solo modo (-t o -p) en caso de que se llame -tp | -pt | - t - p | etc, que seria el primero en ser llamado
	int t_flag = 0, p_flag = 0;
	int N = 0;
	char * archivo_entrada;
	//Aumentamos el indice optind para reservar el primer argumento para el archivo de texto de entrada
	optind = INPUT_ARG_POS + 1;
	if (argc > 0) {
		archivo_entrada = argv[INPUT_ARG_POS];
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
				N = atoi(optarg);
				break;
		}
	}

	if (!validate_params(archivo_entrada, t_flag, p_flag, N))
		return 1;

	printf("Good shit!\n");
	LIST* lista_numeros = exportNumbers(archivo_entrada);

	LIST* d = lista_numeros;
	//TODO: borrar esto, nada mas imprimimos para probar
	while (d) {
		printf("%d\n", d->n);
		d = d->next;
	}
	return 0;
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
	//TODO: Reescribir funcion usando funcion access()
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
