main: server.o file.o error.o parser.o main.o
	gcc server.o file.o error.o parser.o main.o -o main
	rm *.o

error.o: error.c error.h
	gcc -c error.c -o error.o

file.o: file.c error.h file.h
	gcc -c file.c -o file.o

main.o: main.c error.h file.h parser.h server.h
	gcc -c main.c -o main.o

parser.o: parser.c parser.h server.h error.h
	gcc -c parser.c -o parser.o

server.o: server.c server.h parser.h error.h
	gcc -c server.c -o server.o