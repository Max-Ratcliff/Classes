#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 1
#
# make                   makes Lex
# make ListClient        makes ListClient
# make ListTest          makes ListTest
# make all        		 makes all 3 executibles
# make clean             removes all binaries
# make checkClient       checks ListClient for memory errors
# make checkClient       checks ListTest for memory errors
# make checkLex          checks Lex for memory errors
#------------------------------------------------------------------------------
CC=gcc
CFLAGS=-std=c17 -Wall
#CFLAGS+=-D_POSIX_C_SOURCE=200809L # attempt to fix conflic with index() function
#CFLAGS+=-Wextra -Wstrict-prototypes -Werror -pedantic
#CFLAGS+=-g -fsanitize=address,undefined 
.PHONY: Lex ListClient ListTest all clean 

Lex : Lex.o List.o
	$(CC) $(CFLAGS) -o Lex Lex.o List.o

Lex.o : Lex.c List.h
	$(CC) $(CFLAGS) -c Lex.c

ListClient: ListClient.o List.o
	$(CC) $(CFLAGS) -o ListClient ListClient.o List.o

ListClient.o : ListClient.c List.h
	$(CC) $(CFLAGS) -c ListClient.c

ListTest: ListTest.o List.o
	$(CC) $(CFLAGS) -o ListTest ListTest.o List.o

ListTest.o : ListTest.c List.h
	$(CC) $(CFLAGS) -c ListTest.c

List.o : List.c List.h
	$(CC) $(CFLAGS) -c List.c

all: Lex ListClient ListTest

clean :
	rm -f Lex ListClient ListTest *.o

checkClient : 
	valgrind --leak-check=full ./ListClient

checkTest : 
	valgrind --leak-check=full ./ListTest

checkLex :
	valgrind --leak-check=full ./Lex in3 blah3