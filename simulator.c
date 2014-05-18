#include "util.h"
#include "robin-backend.h"
#include "sjf-backend.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

OS200_LOCK_NEW(stderr);

OS200_SYNC_NEW(robin);
OS200_SYNC_NEW(sjf);
OS200_SYNC_NEW(result);

int quit;
char *filename;
os200_result result;

void *robin_loop(void *extra) {
	OS200_LOCK_ACQUIRE(robin);
	while (1) {
		OS200_SYNC_WAIT(robin);
		OS200_SYNC_RESET(robin);
		if (quit)
			break;
		OS200_SYNC_SET_SIGNAL(result, os200_robin_file(filename));
	}
	OS200_UNUSED(extra);
	return NULL;
}

void *sjf_loop(void *extra) {
	OS200_LOCK_ACQUIRE(sjf);
	while (1) {
		OS200_SYNC_WAIT(sjf);
		OS200_SYNC_RESET(sjf);
		if (quit)
			break;
		OS200_SYNC_SET_SIGNAL(result, os200_sjf_file(filename));
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

	filename = os200_read_line(prompt);
	while (strlen(filename) && strcmp(filename, "QUIT")) {
		OS200_SYNC_SIGNAL(sjf);
		OS200_SYNC_SIGNAL(robin);

		OS200_SYNC_WAIT(result);
		os200_result_print(result);
		OS200_SYNC_RESET(result);

		OS200_SYNC_WAIT(result);
		os200_result_print(result);
		OS200_SYNC_RESET(result);

		free(filename);
		filename = os200_read_line(prompt);
	}
	quit = 1;
	OS200_SYNC_SIGNAL(sjf);
	OS200_SYNC_SIGNAL(robin);
	free(filename);
	putchar('\n');

	pthread_join(sjf_thread, NULL);
	pthread_join(robin_thread, NULL);

	return 0;
}
