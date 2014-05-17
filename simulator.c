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
int quit;

void *robin_loop(void *extra) {
	while (1) {
		OS200_WAIT(robin);
		if (quit) {
			OS200_RESET(robin);
			break;
		}
		OS200_LOCK(robin_result);
		robin_result = os200_robin_file(filename);
		OS200_SIGNAL(robin_result);
		OS200_RESET(robin);
	}
	OS200_UNUSED(extra);
	return NULL;
}

void *sjf_loop(void *extra) {
	while (1) {
		OS200_WAIT(sjf);
		if (quit) {
			OS200_RESET(sjf);
			break;
		}
		OS200_LOCK(sjf_result);
		sjf_result = os200_sjf_file(filename);
		OS200_SIGNAL(sjf_result);
		OS200_RESET(sjf);
	}
	OS200_UNUSED(extra);
	return NULL;
}

int main(void) {
	const char prompt[] = "Simulation";
	pthread_t robin_thread;
	pthread_t sjf_thread;

	pthread_create(&robin_thread, NULL, robin_loop, NULL);
	pthread_create(&sjf_thread, NULL, sjf_loop, NULL);

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
	quit = 1;
	OS200_SIGNAL(sjf);
	OS200_SIGNAL(robin);
	free(filename);
	putchar('\n');

	pthread_join(sjf_thread, NULL);
	pthread_join(robin_thread, NULL);
	
	return 0;
}
