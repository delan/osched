#ifndef OS200_RESULT_H
#define OS200_RESULT_H

#include "list.h"
#include "job.h"

typedef enum os200_result_scheduler {
	OS200_SCHEDULER_ROBIN,
	OS200_SCHEDULER_SJF
} os200_result_scheduler;

typedef struct os200_result {
	os200_result_scheduler scheduler;
	double average_waiting;
	double average_turnaround;
} os200_result;

os200_result os200_result_get(os200_list jobs);

void os200_result_print(os200_result result);

#endif
