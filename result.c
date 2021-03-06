#include "result.h"

#include <stdio.h>

typedef struct os200_result_sums {
	double sum_turnaround;
	double sum_waiting;
} os200_result_sums;

void os200_result_step(void *data, void *extra) {
	os200_job job = (os200_job) data;
	os200_result_sums *sums = (os200_result_sums *) extra;
	sums->sum_turnaround +=
		job->completion -
		job->arrival;
	sums->sum_waiting +=
		job->completion -
		job->arrival -
		job->duration;
}

os200_result os200_result_get(os200_list jobs) {
	os200_result result;
	os200_result_sums sums = { 0.0, 0.0 };
	os200_list_foreach(jobs, os200_result_step, &sums);
	result.average_turnaround =
		sums.sum_turnaround /
		jobs->count;
	result.average_waiting =
		sums.sum_waiting /
		jobs->count;
	return result;
}

void os200_result_print(os200_result result) {
	char *scheduler_prefix;
	switch (result.scheduler) {
	case OS200_SCHEDULER_ROBIN:
		scheduler_prefix = "RR:  ";
		break;
	case OS200_SCHEDULER_SJF:
		scheduler_prefix = "SJF: ";
		break;
	}
	printf(
		"%s average turnaround: %f, average waiting: %f\n",
		scheduler_prefix,
		result.average_turnaround,
		result.average_waiting
	);
}

