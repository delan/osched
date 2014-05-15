#ifndef OS200_JOB_H
#define OS200_JOB_H

#include <stdio.h>
#include "list.h"

typedef struct os200_job {
	double duration;
	double arrival;
	double completion;
	double remaining;
} *os200_job;

os200_job os200_job_new(double arrival, double duration);

void os200_job_free(os200_job job);

int os200_job_compare_arrival_duration(void *a, void *b);

int os200_job_compare_arrival(void *a, void *b);

int os200_job_compare_duration(void *a, void *b);

double os200_job_scan(FILE *input, os200_list output);

#endif
