primos.o: primos.c
	gcc -c primos.c -o primos.o
	gcc primos.o -o primos

primos: primos.o
	gcc primos.o -o primos

clean:
	-rm -f primos.o
	-rm -f primos

run: primos
	./primos -n 5
