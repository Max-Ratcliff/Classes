CC = clang
CFLAGS = -Werror -Wall -Wextra -Wconversion -Wdouble-promotion -Wstrict-prototypes -pedantic
CFLAGS += -gdwarf-4
LFLAGS = -lm


all: calc


calc: calc.o stack.o mathlib.o operators.o
	$(CC) $^ $(LFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -rf calc *.o

format:
	clang-format -i -style=file *.[ch]

.PHONY: all clean format

