
main: main.o libcalculator.a
	gcc main.o -o main -L. -lcalculator 
main.o: main.c
	gcc main.c -c

libcalculator.a: addition.o substraction.o multiplication.o division.o
	ar rc $@ addition.o substraction.o multiplication.o division.o
addition.o: addition.c
	gcc addition.c -c
substraction.o: substraction.c
	gcc substraction.c -c
multiplication.o: multiplication.c
	gcc multiplication.c -c
division.o: division.c
	gcc division.c -c
