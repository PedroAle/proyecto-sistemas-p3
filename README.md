# Proyecto Sistemas de Operacion - Problema 1

Programa que recibe como entrada una lista de N enteros positivos n0, n1, ..., nN-1 y determina si cada uno de estos enteros ni es primo o no. El objetivo de este problema es que realice una evaluación de desempeño del programa, donde compare el tiempo de corrida utilizando diferente número de procesos e hilos en Linux.

## LLamada

`primos entrada.txt [-t | -p] [-n N]`

* entrada.txt es el nombre de archivo de entrada
* `-p` o `-t` indican que el programa deberá utilizar procesos o hilos, respectivamente.
* `N` es el número de procesos/hilos a crear. Siempre se debe cumplir 1 ≤ N ≤ 10
* El archivo de entrada es un archivo de texto donde cada línea tendrá un entero positivo. El archivo termina en fin de línea.


## Medir tiempo

`./medir_tiempo.sh [t | p] N`

* `t` o `p` para threads o procesos (Nota: no lleva `-` antes de la letra)
* `N` es el número de procesos/hilos a crear

Este script llamara al programa `primos` con un archivo llamado `input.txt` donde deben estar los numeros, por lo que se debe guardar el archivo de prueba como `input.txt` para que el script funcione.
Genera un archivo de texto llama `[t | p]N.txt` que contiene los tiempos medidos con la instruccion `time`
