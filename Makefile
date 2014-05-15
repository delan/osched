CFLAGS=-std=c99 -Werror -Wall -Wextra -pedantic -ggdb

all: robin sjf

robin: robin-backend.o util.o list.o job.o result.o

sjf: sjf-backend.o util.o list.o job.o result.o

robin-backend.o: robin-backend.c robin-backend.h list.h job.h

sjf-backend.o: sjf-backend.c sjf-backend.h list.h job.h

util.o: util.c util.h

list.o: list.c list.h

job.o: job.c job.h list.h

result.o: result.c result.h list.h job.h

clean:
	rm -f *.o
