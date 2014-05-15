#ifndef OS200_RESULT_H
#define OS200_RESULT_H

#include "list.h"
#include "job.h"

typedef struct os200_result {
	double average_waiting;
	double average_turnaround;
} os200_result;

os200_result os200_result_get(os200_list jobs);

void os200_result_print(os200_result result);

#endif
