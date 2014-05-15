CFLAGS=-std=c99 -Werror -Wall -Wextra -pedantic -g
BINARIES=robin sjf
OBJECTS_robin=robin.o robin-backend.o util.o list.o job.o result.o
OBJECTS_sjf=sjf.o sjf-backend.o util.o list.o job.o result.o

all: binaries report.pdf

binaries: $(BINARIES)

robin: $(OBJECTS_robin)
	$(CC) -o robin $(OBJECTS_robin)

sjf: $(OBJECTS_sjf)
	$(CC) -o sjf $(OBJECTS_sjf)

report.pdf:
	pdflatex report.tex

clean:
	rm -f *.o *.exe *.aux *.out *.toc *.pdf *.log $(BINARIES)
