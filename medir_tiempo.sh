#!/bin/bash
filename="Resultados/Time/$1$2.txt"

echo Timepo guardado en: $filename
cd Resultados
mkdir Time
cd ..

for i in `seq 1 10`; do
  (time ./primos entrada.txt -$1 -n $2) 2>> $filename
done
