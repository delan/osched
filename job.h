#ifndef OS200_JOB_H
#define OS200_JOB_H

typedef struct os200_job {
	double duration;
	double arrival;
	double completion;
	double remaining;
} *os200_job;

os200_job os200_job_new(double arrival, double duration);

double os200_job_scan(FILE *input, os200_list output);

#endif
