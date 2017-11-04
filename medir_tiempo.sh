#!/bin/bash

mkdir Resultados
cd Resultados
mkdir Time
cd ..

for j in `seq 1 10`; do
  printf "\nNumero de procesos: $j\n"

    for i in `seq 1 10`; do
      printf "\nCorrida $i\n"
      echo "-------- $i ---------" >> "Resultados/Time/$2$j.txt" #$filename
      (time ./primos "$1" -$2 -n $j) 2>> "Resultados/Time/$2$j.txt"
    done
done
