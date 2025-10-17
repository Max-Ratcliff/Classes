# EXEC     = queue
SOURCES  = $(wildcard *.c) # matches all .c files in the current directory
OBJECTS  = $(SOURCES:%.c=%.o) # replaces .c with .o to get object files
FORMATS  = $(SOURCES:%.c=%.fmt) # replaces .c with .fmt for formatting

CC       = clang
FORMAT   = clang-format
CFLAGS   = -Wall -Werror -Wextra -Wpedantic -Wstrict-prototypes
# additional flags for debugging
# CFLAGS  += -g -fsanitize=address -fsanitize=undefined -Wconversion -Wshadow -Wundef 

.PHONY: all clean format

all: $(OBJECTS)

# dont need an executable for this assignment
# build the executable from object and library files
# The $@ is the target (the executable)
# The $^ is all the prerequisites (the object files)
# $(EXEC): $(OBJECTS) 
#	 $(CC) $(CFLAGS) -o $@ $^

# build object files from source files
# The $< is the first prerequisite (the source file)
%.o : %.c
	$(CC) $(CFLAGS) -c $<

# clean executable and object files
clean:
	rm -f $(OBJECTS)


format: $(FORMATS)

# clang format the source files
%.fmt: %.c
	$(FORMAT) -i $<
	echo $@
