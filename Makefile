CFLAGS=-ansi -Werror -Wall -Wextra -pedantic

all: util.o

util.o: util.c util.h

clean:
	rm -f *.o
