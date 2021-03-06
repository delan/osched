#include "job.h"

#include <stdlib.h>
#include "util.h"

os200_job os200_job_new(double arrival, double duration) {
	os200_job job = malloc(sizeof(struct os200_job));
	job->duration = duration;
	job->arrival = arrival;
	job->completion = 0.0 / 0.0; /* IEEE 754 NaN */
	job->remaining = duration;
	return job;
}

void os200_job_free(os200_job job) {
	free(job);
}

void os200_job_free_in_list(void *data, void *extra) {
	os200_job job = data;
	os200_job_free(job);
	OS200_UNUSED(extra);
}

int os200_job_compare_arrival(void *a, void *b) {
	int result = 0;
	os200_job x = (os200_job) a;
	os200_job y = (os200_job) b;
	if (x->arrival > y->arrival)
		result = 1;
	if (x->arrival < y->arrival)
		result = -1;
	return result;
}

int os200_job_compare_duration(void *a, void *b) {
	int result = 0;
	os200_job x = (os200_job) a;
	os200_job y = (os200_job) b;
	if (x->duration > y->duration)
		result = 1;
	if (x->duration < y->duration)
		result = -1;
	return result;
}

int os200_job_compare_arrival_duration(void *a, void *b) {
	int result;
	int arrival_comparison = os200_job_compare_arrival(a, b);
	int duration_comparison = os200_job_compare_duration(a, b);
	if (arrival_comparison)
		result = arrival_comparison;
	else
		result = duration_comparison;
	return result;
}

double os200_job_scan(FILE *input, os200_list output) {
	double quantum, arrival, duration;
	fscanf(input, "%lf", &quantum);
	while (fscanf(input, "%lf%lf", &arrival, &duration) == 2) {
		os200_job job = os200_job_new(arrival, duration);
		os200_list_insert_sorted(output, job,
			os200_job_compare_arrival);
	}
	return quantum;
}
