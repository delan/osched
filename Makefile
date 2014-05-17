CFLAGS=-std=c99 -Werror -Wall -Wextra -pedantic -g
LDLIBS=-lpthread

CFLAGS_TSAN=-std=c99 -Werror -Wall -Wextra -pedantic -g -fsanitize=thread -fPIE
LDFLAGS_TSAN=-fsanitize=thread -pie

BINARIES=robin sjf simulator

OBJECTS_common = util.o list.o job.o result.o
OBJECTS_robin=robin.o robin-backend.o $(OBJECTS_common)
OBJECTS_sjf=sjf.o sjf-backend.o $(OBJECTS_common)
OBJECTS_simulator=simulator.o robin-backend.o sjf-backend.o $(OBJECTS_common)

all: binaries report.pdf

binaries: $(BINARIES)

robin: $(OBJECTS_robin)
	$(CC) $(LDFLAGS) $(OBJECTS_robin) $(LDLIBS) -o robin

sjf: $(OBJECTS_sjf)
	$(CC) $(LDFLAGS) $(OBJECTS_sjf) $(LDLIBS) -o sjf

simulator: $(OBJECTS_simulator)
	$(CC) $(LDFLAGS) $(OBJECTS_simulator) $(LDLIBS) -o simulator

report.pdf: report.tex
	pdflatex report.tex

clean:
	rm -f *.o *.exe *.aux *.out *.toc *.pdf *.log $(BINARIES)
