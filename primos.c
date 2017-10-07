#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int validate_params(int t_flag, int p_flag, int N);

int main(int argc, char *argv[]) {
	printf("Proyecto Sistemas de Operacion - Problema 1\n");
	int c;
	//Estos flags permiten determinar un solo modo (-t o -p) en caso de que se llame -tp | -pt | - t - p | etc, que seria el primero en ser llamado
	int t_flag = 0, p_flag = 0;
	int N = 0;
	//Aumentamos el indice optind para reservar el primer argumento para el archivo de texto de entrada
	optind = 2;
	//TODO: Aqui deberiamos extraer el archivo de entrada y despues seguir analizando el resto de los argumentos

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
				//TODO: Validar que 1 <= N <= 10
				N = atoi(optarg);
				break;

			case '?':
				printf("%s\n", optarg);
				break;
		}
	} 

	if (!validate_params(t_flag,p_flag,N))
		return 1;

	return 0;
}

int validate_params(int t_flag, int p_flag, int N) {
	/* Valida los argumentos necesarios, devuelve 1 si son validos y 0 si no lo son */

	//TODO: Validar que el archivo de entrada exista

	if (t_flag == 0 && p_flag == 0) {
		printf("./primos: -t or -p argument required.\n( primos entrada.txt [-t | -p] [-n N] )\n");
		return 0;
	}

	if (N == 0) {
		printf("./primos: -n N argument required. 1 <= N <= 10\n( primos entrada.txt [-t | -p] [-n N] )\n");
		return 0;
	}

	return 1;
}
