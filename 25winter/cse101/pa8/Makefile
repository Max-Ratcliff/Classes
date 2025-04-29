#------------------------------------------------------------------------------
#  Makefile for CSE 101 Programming Assignment 8
#
#  Clients:
#     Order              - Main client
#     DictionaryClient   - Dictionary client
#     DictionaryTest     - Dictionary test client
#     WordFrequency      - Word frequency analysis client
#
#  Dependency:
#     Dictionary       - Dictionary implementation (Dictionary.cpp and Dictionary.h)
#
#  Targets:
#     all, client, test, clean, check
#------------------------------------------------------------------------------

MAIN           = WordFrequency
MAIN2		   = Order
DEP1           = Dictionary
COMPILER       = g++
FLAGS          = -std=c++17 -Wall
REMOVE         = rm -f
VALGRIND       = valgrind --leak-check=full
FLAGS += -Wextra -Wpedantic 
FLAGS += -O0 -g -fsanitize=address,undefined 
VALGRIND += --tool=memcheck --show-leak-kinds=all --track-origins=yes --verbose -v

.PHONY: $(MAIN) test client all clean check

# ------ Build Main Program ------
$(MAIN): $(MAIN).o $(DEP1).o 
	$(COMPILER) $(FLAGS) -o $(MAIN) $(MAIN).o $(DEP1).o

$(MAIN).o: $(MAIN).cpp $(DEP1).h 
	$(COMPILER) $(FLAGS) -c $(MAIN).cpp

# ------ Build 2nd Main Program ------
$(MAIN2): $(MAIN2).o $(DEP1).o 
	$(COMPILER) $(FLAGS) -o $(MAIN2) $(MAIN2).o $(DEP1).o

$(MAIN2).o: $(MAIN2).cpp $(DEP1).h 
	$(COMPILER) $(FLAGS) -c $(MAIN2).cpp

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

# # ------ Build Second Dependancies Test and Clients ------
# # Client
# $(DEP2)Client: $(DEP2)Client.o $(DEP1).o $(DEP2).o
# 	$(COMPILER) $(FLAGS) -o $(DEP2)Client $(DEP2)Client.o $(DEP1).o $(DEP2).o

# $(DEP2)Client.o: $(DEP2)Client.cpp $(DEP1).h $(DEP2).h
# 	$(COMPILER) $(FLAGS) -c $(DEP2)Client.cpp

# # Test
# $(DEP2)Test: $(DEP2)Test.o $(DEP1).o $(DEP2).o
# 	$(COMPILER) $(FLAGS) -o $(DEP2)Test $(DEP2)Test.o $(DEP1).o $(DEP2).o

# $(DEP2)Test.o: $(DEP2)Test.cpp $(DEP1).h $(DEP2).h
# 	$(COMPILER) $(FLAGS) -c $(DEP2)Test.cpp

# # Object
# $(DEP2).o: $(DEP2).cpp $(DEP1).h $(DEP2).h
# 	$(COMPILER) $(FLAGS) -c $(DEP2).cpp

# ------ Utility ------

# Make all Executables
all: $(MAIN) $(MAIN2) $(DEP1)Test $(DEP1)Client # $(DEP2)Client

# Make all Client Executables
client: $(DEP1)Client 

# Make all Test Executables
test: $(DEP1)Test 

check: checkClient checkTest check$(MAIN) check$(MAIN2)

# delete all executables and Binaries
clean:
	$(REMOVE) $(MAIN) $(MAIN2) $(DEP1)Test $(DEP1)Client *.o

# Check for memory leaks
checkClient: 
	$(VALGRIND) ./$(DEP1)Client 


checkTest: 
	$(VALGRIND) ./$(DEP1)Test 


check$(MAIN):
	$(VALGRIND) ./$(MAIN) ./tests/in1 ./tests/vOut1

check$(MAIN2):
	$(VALGRIND) ./$(MAIN2) ./tests/in1 ./tests/vOut1