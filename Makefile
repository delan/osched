CFLAGS=-std=c99 -Werror -Wall -Wextra -pedantic -g
BINARIES=robin sjf

all: binaries report.pdf

binaries: $(BINARIES)

robin: robin.o robin-backend.o util.o list.o job.o result.o

sjf: sjf.o sjf-backend.o util.o list.o job.o result.o

robin.o: robin.c

sjf.o: sjf.c

robin-backend.o: robin-backend.c

sjf-backend.o: sjf-backend.c

util.o: util.c

list.o: list.c

job.o: job.c

result.o: result.c

report.pdf:
	pdflatex report.tex

clean:
	rm -f *.o *.exe *.aux *.out *.toc *.pdf *.log $(BINARIES)
