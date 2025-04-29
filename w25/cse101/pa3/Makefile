#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 2
#
# make                   makesFindComponents 
# make FindComponents    makesFindComponents 
# make GraphTest         makes GraphTest
# make all        		 makes all 3 executibles
# make clean             removes all binaries
# make checkClient       checks GraphClient for memory errors
# make checkTest       	 checks GraphTest for memory errors
# make checkPath         checks FindComponents for memory errors
#------------------------------------------------------------------------------
CC=gcc
CFLAGS=-std=c17 -Wall
#CFLAGS+=-D_POSIX_C_SOURCE=200809L # attempt to fix conflic with index() function
#CFLAGS+=-Wextra -Wstrict-prototypes -Werror -pedantic
#CFLAGS+=-g -fsanitize=address,undefined 
.PHONY: FindComponents GraphTest GraphClient all clean 

FindComponents : FindComponents.o Graph.o List.o
	$(CC) $(CFLAGS) -o FindComponents FindComponents.o Graph.o List.o

FindComponents.o : FindComponents.c Graph.h List.h
	$(CC) $(CFLAGS) -c FindComponents.c

GraphClient: GraphClient.o Graph.o List.o
	$(CC) $(CFLAGS) -o GraphClient GraphClient.o Graph.o List.o

GraphClient.o: GraphClient.c Graph.h List.h
	$(CC) $(CFLAGS) -c GraphClient.c

GraphTest: GraphTest.o Graph.o List.o
	$(CC) $(CFLAGS) -o GraphTest GraphTest.o Graph.o List.o

GraphTest.o: GraphTest.c Graph.h List.h
	$(CC) $(CFLAGS) -c GraphTest.c

Graph.o : Graph.c Graph.h List.h
	$(CC) $(CFLAGS) -c Graph.c

List.o : List.c List.h
	$(CC) $(CFLAGS) -c List.c

all: FindComponents GraphTest GraphClient

clean :
	rm -f FindComponents GraphTest GraphClient *.o

checkClient : 
	valgrind --leak-check=full ./GraphClient

checkTest : 
	valgrind --leak-check=full ./GraphTest

checkPath :
	valgrind --leak-check=full ./FindComponents in3 blah3