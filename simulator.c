#include "util.h"
#include "robin-backend.h"
#include "sjf-backend.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

OS200_LOCKED_GLOBAL(stderr);

OS200_NEW_SYNCHRONISED_GLOBAL(os200_result, robin_result);
OS200_NEW_SYNCHRONISED_GLOBAL(os200_result, sjf_result);
OS200_NEW_SYNCHRONISED_GLOBAL(char *, filename);

void *worker(void *extra) {
	os200_scheduler scheduler = * (os200_scheduler *) extra;
	while (1) {
		OS200_WAIT(filename);
		char *filename_copy = os200_strdup(filename);
		OS200_UNLOCK(filename);

		switch (scheduler) {
		case OS200_SCHEDULER_ROBIN:
			OS200_LOCK(robin_result);
			robin_result = os200_robin_file(filename_copy);
			OS200_SIGNAL(robin_result);
			break;
		case OS200_SCHEDULER_SJF:
			OS200_LOCK(sjf_result);
			sjf_result = os200_sjf_file(filename_copy);
			OS200_SIGNAL(sjf_result);
			break;
		}

		free(filename_copy);
	}
	return NULL;
}

int main(void) {
	const char prompt[] = "Simulation";
	os200_scheduler robin_scheduler = OS200_SCHEDULER_ROBIN;
	os200_scheduler sjf_scheduler = OS200_SCHEDULER_SJF;
	pthread_t robin_thread;
	pthread_t sjf_thread;

	pthread_create(&robin_thread, NULL, worker, &robin_scheduler);
	pthread_create(&sjf_thread, NULL, worker, &sjf_scheduler);

	pthread_detach(robin_thread);
	pthread_detach(sjf_thread);

	OS200_LOCK(filename);
	filename = os200_read_line(prompt);
	while (strlen(filename) && strcmp(filename, "QUIT")) {
		OS200_SIGNAL(filename);
		OS200_WAIT(robin_result);
		OS200_WAIT(sjf_result);

		puts("\nFor the Round Robin scheduler:");
		os200_result_print(robin_result);
		puts("\nFor the Shortest Job First scheduler:");
		os200_result_print(sjf_result);
		putchar('\n');

		OS200_RESET(filename);
		OS200_RESET_UNLOCK(robin_result);
		OS200_RESET_UNLOCK(sjf_result);
		OS200_LOCK(filename);
		free(filename);
		filename = os200_read_line(prompt);
	}
	free(filename);
	return 0;
}
