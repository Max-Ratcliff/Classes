#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 2
#
# make                   makes Sparse  
# make Sparse     		 makes Sparse  
# make MatrixTest        makes MatrixTest 
# make all        		 makes all 3 executibles
# make clean             removes all binaries
# make checkClient       checks MatrixClient and List Client for memory errors
# make checkTest       	 checks MatrixTest for memory errors
# make checkPath         checks Sparse for memory errors
#------------------------------------------------------------------------------
CC=gcc
CFLAGS=-std=c17 -Wall
# CFLAGS+=-D_POSIX_C_SOURCE=200809L # attempt to fix conflic with index() function
# CFLAGS+=-Wextra -Wstrict-prototypes -Werror -pedantic
# CFLAGS+=-g -fsanitize=address,undefined 
.PHONY: Sparse MatrixTest MatrixClient ListClient all clean 

Sparse: Sparse.o Matrix.o List.o
	$(CC) $(CFLAGS) -o Sparse Sparse.o Matrix.o List.o

Sparse.o: Sparse.c Matrix.h List.h
	$(CC) $(CFLAGS) -c Sparse.c

MatrixClient: MatrixClient.o Matrix.o List.o
	$(CC) $(CFLAGS) -o MatrixClient MatrixClient.o Matrix.o List.o

MatrixClient.o: MatrixClient.c Matrix.h List.h
	$(CC) $(CFLAGS) -c MatrixClient.c

MatrixTest: MatrixTest.o Matrix.o List.o
	$(CC) $(CFLAGS) -o MatrixTest MatrixTest.o Matrix.o List.o

MatrixTest.o: MatrixTest.c Matrix.h List.h
	$(CC) $(CFLAGS) -c MatrixTest.c

Matrix.o: Matrix.c Matrix.h List.h
	$(CC) $(CFLAGS) -c Matrix.c

ListClient: ListClient.o List.o
	$(CC) $(CFLAGS) -o ListClient ListClient.o List.o

ListClientClient.o: ListClient.c List.h
	$(CC) $(CFLAGS) -c ListClient.c

ListTest: ListTest.o List.o
	$(CC) $(CFLAGS) -o ListTest ListTest.o List.o

ListTest.o: ListTest.c List.h
	$(CC) $(CFLAGS) -c ListTest.c

List.o: List.c List.h
	$(CC) $(CFLAGS) -c List.c

all: Sparse MatrixTest ListTest MatrixClient ListClient

client: MatrixClient ListClient

test: MatrixTest ListTest

clean:
	rm -f Sparse MatrixTest ListTest MatrixClient ListClient *.o

checkClient: 
	valgrind --leak-check=full ./MatrixClient
	valgrind --leak-check=full ./ListClient

checkTest: 
	valgrind --leak-check=full ./MatrixTest
	valgrind --leak-check=full ./ListTest

checkSparse:
	valgrind --leak-check=full ./Sparse  in3 blah3