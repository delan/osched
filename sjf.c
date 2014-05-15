#include "util.h"
#include "sjf-backend.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
	const char prompt[] = "SJF simulation";
	os200_result result;
	char *filename = os200_read_line(prompt);
	while (strlen(filename) && strcmp(filename, "QUIT")) {
		result = os200_sjf_file(filename);
		printf(
			"Average waiting time:    %f\n",
			result.average_waiting
		);
		printf(
			"Average turnaround time: %f\n",
			result.average_turnaround
		);
		free(filename);
		filename = os200_read_line(prompt);
	}
	free(filename);
	return 0;
}
