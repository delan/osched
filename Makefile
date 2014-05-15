CFLAGS=-ansi -Werror -Wall -Wextra -pedantic

all: util.o list.o

util.o: util.c util.h

list.o: list.c list.h

clean:
	rm -f *.o
