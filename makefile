primos.o: primos.c
	gcc -c primos.c -o primos.o

primos: primos.o
	gcc primos.o -o primos

clean:
	-rm -f primos.o
	-rm -f primos

run: primos
	./primos