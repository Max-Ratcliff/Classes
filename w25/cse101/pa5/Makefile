#------------------------------------------------------------------------------
# Makefile for CSE 101 Programming Assignment 5
#
# make                   makes Shuffle  
# make Shuffle     		 makes Shuffle  
# make all        		 makes all 3 executibles
# make clean             removes all binaries
# make checkClient       checks ent and List Client for memory errors
# make checkTest       	 checks t for memory errors
# make checkPath         checks Shuffle for memory errors
#------------------------------------------------------------------------------
CPPC=g++
CPPFLAGS=-std=c++17 -Wall
# CPPFLAGS+=-D_POSIX_C_SOURCE=200809L # attempt to fix conflic with index() function
# CPPFLAGS+=-Wextra -Wstrict-prototypes -Werror -pedantic
# CPPFLAGS+=-g -fsanitize=address,undefined 
.PHONY: Shuffle ListTest ListClient all clean 

Shuffle: Shuffle.o List.o
	$(CPPC) $(CPPFLAGS) -o Shuffle Shuffle.o List.o

Shuffle.o: Shuffle.cpp List.h
	$(CPPC) $(CPPFLAGS) -c Shuffle.cpp

ListClient: ListClient.o List.o
	$(CPPC) $(CPPFLAGS) -o ListClient ListClient.o List.o

ListClient.o: ListClient.cpp List.h
	$(CPPC) $(CPPFLAGS) -c ListClient.cpp

ListTest: ListTest.o List.o
	$(CPPC) $(CPPFLAGS) -o ListTest ListTest.o List.o

ListTest.o: ListTest.cpp List.h
	$(CPPC) $(CPPFLAGS) -c ListTest.cpp

List.o: List.cpp List.h
	$(CPPC) $(CPPFLAGS) -c List.cpp

all: Shuffle ListTest ListClient

client: ListClient

test: ListTest

clean:
	rm -f Shuffle ListTest ListClient *.o

checkClient: 
	valgrind --leak-check=full ./ListClient

checkTest: 
	valgrind --leak-check=full ./ListTest

checkShuffle:
	valgrind --leak-check=full ./Shuffle  in3 blah3