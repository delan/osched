CFLAGS=-ansi -Werror -Wall -Wextra -pedantic

all: util.o list.o job.o

util.o: util.c util.h

list.o: list.c list.h

job.o: job.c job.h list.h

clean:
	rm -f *.o
