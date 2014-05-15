#include "util.h"
#include "robin-backend.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	const char prompt[] = "RR simulation";
	os200_result result;
	if (argc > 1) {
		result = os200_robin_file(argv[1]);
		os200_result_print(result);
	} else {
		char *filename = os200_read_line(prompt);
		while (strlen(filename) && strcmp(filename, "QUIT")) {
			result = os200_robin_file(filename);
			os200_result_print(result);
			free(filename);
			filename = os200_read_line(prompt);
		}
		free(filename);	
	}
	return 0;
}
