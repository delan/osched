CFLAGS=-std=c99 -Werror -Wall -Wextra -pedantic -g
LDLIBS=-lpthread

BINARIES=robin sjf simulator

OBJECTS_common = util.o list.o job.o result.o
OBJECTS_robin=robin.o robin-backend.o $(OBJECTS_common)
OBJECTS_sjf=sjf.o sjf-backend.o $(OBJECTS_common)
OBJECTS_simulator=simulator.o robin-backend.o sjf-backend.o $(OBJECTS_common)

all: binaries report.pdf

binaries: $(BINARIES)

robin: $(OBJECTS_robin)
	$(CC) -o robin $(OBJECTS_robin) $(LDLIBS)

sjf: $(OBJECTS_sjf)
	$(CC) -o sjf $(OBJECTS_sjf) $(LDLIBS)

simulator: $(OBJECTS_simulator)
	$(CC) -o simulator $(OBJECTS_simulator) $(LDLIBS)

report.pdf: report.tex
	pdflatex report.tex

clean:
	rm -f *.o *.exe *.aux *.out *.toc *.pdf *.log $(BINARIES)
