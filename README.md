# Proyecto Sistemas de Operacion - Problema 1

Programa que recibe como entrada una lista de N enteros positivos n0, n1, ..., nN-1 y determina si cada uno de estos enteros ni es primo o no. El objetivo de este problema es que realice una evaluación de desempeño del programa, donde compare el tiempo de corrida utilizando diferente número de procesos e hilos en Linux.

## LLamada

`primos entrada.txt [-t | -p] [-n N]`

* `entrada.txt` es el nombre de archivo de entrada
* `-p` o `-t` indican que el programa deberá utilizar procesos o hilos, respectivamente.
* `N` es el número de procesos/hilos a crear. Siempre se debe cumplir 1 ≤ N ≤ 10
* El archivo de entrada es un archivo de texto donde cada línea tendrá un entero positivo. El archivo termina en fin de línea.


## Medir tiempo

`./medir_tiempo.sh entrada.txt [t | p]`

* `entrada.txt` es el nombre de archivo de entrada
* `t` o `p` para threads o procesos (Nota: no lleva `-` antes de la letra)

Este script correra el, programa `primos` 10 veces de 1 a 10 threads.

Crea una carpeta `Resultados/Time` y dentro de esta guarda todos lo resultados de tiempo de 10 corridas con N procesos.
