primos.o: primos.c
	gcc -c primos.c -o primos.o -lpthread
	gcc primos.o -o primos -lpthread

primos: primos.o
	gcc primos.o -o primos

clean:
	-rm -f primos.o
	-rm -f primos
