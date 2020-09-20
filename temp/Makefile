CC=gcc

testlib : console.o list.o bitmap.o hash.o
	gcc -g -o testlib console.o list.o bitmap.o hash.o

console.o : console.c console.h list.h bitmap.h hash.h
	gcc -c -w console.c 

list.o : list.h debug.h
	gcc -c list.c

bitmap.o : bitmap.h limits.h round.h stdio.c
	gcc -c bitmap.c

hash.o : hash.h debug.h
	gcc -c hash.c

clean :
	rm *.o testlib
