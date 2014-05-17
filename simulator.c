#include "util.h"
#include "robin-backend.h"
#include "sjf-backend.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

OS200_LOCKED_GLOBAL(stderr);

OS200_SYNCHRONISED_GLOBAL(robin);
OS200_SYNCHRONISED_GLOBAL(sjf);

OS200_NEW_SYNCHRONISED_GLOBAL(os200_result, robin_result);
OS200_NEW_SYNCHRONISED_GLOBAL(os200_result, sjf_result);

char *filename;

void *worker(void *extra) {
	os200_scheduler scheduler = * (os200_scheduler *) extra;
	while (1) {
		switch (scheduler) {
		case OS200_SCHEDULER_ROBIN:
			OS200_WAIT(robin);
			OS200_LOCK(robin_result);
			robin_result = os200_robin_file(filename);
			OS200_SIGNAL(robin_result);
			OS200_RESET(robin);
			break;
		case OS200_SCHEDULER_SJF:
			OS200_WAIT(sjf);
			OS200_LOCK(sjf_result);
			sjf_result = os200_sjf_file(filename);
			OS200_SIGNAL(sjf_result);
			OS200_RESET(sjf);
			break;
		}
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

	OS200_LOCK(robin);
	OS200_LOCK(sjf);
	filename = os200_read_line(prompt);
	while (strlen(filename) && strcmp(filename, "QUIT")) {
		OS200_SIGNAL(sjf);
		OS200_SIGNAL(robin);
		OS200_WAIT(robin_result);
		OS200_WAIT(sjf_result);

		fputs("RR:  ", stdout);
		os200_result_print(robin_result);
		fputs("SJF: ", stdout);
		os200_result_print(sjf_result);

		OS200_RESET(sjf_result);
		OS200_RESET(robin_result);
		OS200_LOCK(robin);
		OS200_LOCK(sjf);
		free(filename);
		filename = os200_read_line(prompt);
	}
	free(filename);
	return 0;
}
