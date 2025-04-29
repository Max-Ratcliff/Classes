#------------------------------------------------------------------------------
#  Makefile for CSE 101 Programming Assignment 6
#
#  make                     makes Arithmetic
#  make BigIntegerClient    makes BigIntegerClient
#  make ListClient          makes ListClient
#  make clean               removes all binaries
#  make ArithmeticCheck     runs Arithmetic in valgrind on in4 junk4
#  make BigIntegerCheck     runs BigIntegerTest in valgrind
#  make ListCheck           runs ListTest in valgrind
#------------------------------------------------------------------------------
MAIN           = Arithmetic
DEP1           = List
DEP2           = BigInteger
COMPILER       = g++
FLAGS          = -std=c++17 -Wall
REMOVE         = rm -f
VALGRIND       = valgrind --leak-check=full
FLAGS += -Wextra -Wpedantic 
FLAGS += -O0 -g -fsanitize=address,undefined 
VALGRIND += --tool=memcheck --show-leak-kinds=all --track-origins=yes --verbose -v

.PHONY: $(MAIN) test client all clean check checkClient checkTest check$(MAIN) runModel

# ------ Build Main Program ------
$(MAIN): $(MAIN).o $(DEP1).o $(DEP2).o
	$(COMPILER) $(FLAGS) -o $(MAIN) $(MAIN).o $(DEP1).o $(DEP2).o

$(MAIN).o: $(MAIN).cpp $(DEP1).h $(DEP2).h
	$(COMPILER) $(FLAGS) -c $(MAIN).cpp

# ------ Build First Dependancies Test and Clients --------
# Client
$(DEP1)Client: $(DEP1)Client.o $(DEP1).o
	$(COMPILER) $(FLAGS) -o $(DEP1)Client $(DEP1)Client.o $(DEP1).o

$(DEP1)Client.o: $(DEP1)Client.cpp $(DEP1).h
	$(COMPILER) $(FLAGS) -c $(DEP1)Client.cpp

#Test
$(DEP1)Test: $(DEP1)Test.o $(DEP1).o
	$(COMPILER) $(FLAGS) -o $(DEP1)Test $(DEP1)Test.o $(DEP1).o

$(DEP1)Test.o: $(DEP1)Test.cpp $(DEP1).h
	$(COMPILER) $(FLAGS) -c $(DEP1)Test.cpp

#object
$(DEP1).o: $(DEP1).cpp $(DEP1).h
	$(COMPILER) $(FLAGS) -c $(DEP1).cpp

# ------ Build Second Dependancies Test and Clients ------
# Client
$(DEP2)Client: $(DEP2)Client.o $(DEP1).o $(DEP2).o
	$(COMPILER) $(FLAGS) -o $(DEP2)Client $(DEP2)Client.o $(DEP1).o $(DEP2).o

$(DEP2)Client.o: $(DEP2)Client.cpp $(DEP1).h $(DEP2).h
	$(COMPILER) $(FLAGS) -c $(DEP2)Client.cpp

# Test
$(DEP2)Test: $(DEP2)Test.o $(DEP1).o $(DEP2).o
	$(COMPILER) $(FLAGS) -o $(DEP2)Test $(DEP2)Test.o $(DEP1).o $(DEP2).o

$(DEP2)Test.o: $(DEP2)Test.cpp $(DEP1).h $(DEP2).h
	$(COMPILER) $(FLAGS) -c $(DEP2)Test.cpp

# Object
$(DEP2).o: $(DEP2).cpp $(DEP1).h $(DEP2).h
	$(COMPILER) $(FLAGS) -c $(DEP2).cpp

# ------ Utility ------

# Make all Executables
all: $(MAIN) $(DEP1)Test $(DEP1)Client $(DEP2)Test $(DEP2)Client

# Make all Client Executables
client: $(DEP1)Client $(DEP2)Client

# Make all Test Executables
test: $(DEP1)Test $(DEP2)Test

check: checkClient checkTest check$(MAIN)

# delete all executables and Binaries
clean:
	$(REMOVE) $(MAIN) $(DEP1)Test $(DEP1)Client $(DEP2)Test $(DEP2)Client *.o

# Check for memory leaks
checkClient: 
	$(VALGRIND) ./$(DEP1)Client 
	$(VALGRIND) ./$(DEP2)Client

checkTest: 
	$(VALGRIND) ./$(DEP1)Test 
	$(VALGRIND) ./$(DEP2)Test

check$(MAIN):
	$(VALGRIND) ./$(MAIN) ./tests/in1 ./tests/vOut1

runModel:
	g++ -std=c++17 -Wall -c -g ./ModelListTest.cpp List.cpp
	g++ -std=c++17 -Wall -o ModelListTest ModelListTest.o List.o
	valgrind --leak-check=full -v ./ModelListTest -v > ListTest-out.txt 2> ListTest-mem.txt