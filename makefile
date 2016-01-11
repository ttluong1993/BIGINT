all:	bigint.o main.o
	gcc bigint.o main.o -o test -lm
bigint.o:	bigint.c
	gcc -c bigint.c
main.o:	main.c
	gcc -c main.c
clean:
	rm -f *.o test
