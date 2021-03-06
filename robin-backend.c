#include "robin-backend.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "util.h"
#include "job.h"

os200_result os200_robin_file(const char *filename) {
	double quantum;
	FILE *input;
	os200_result result = { OS200_SCHEDULER_ROBIN, 0.0 / 0.0, 0.0 / 0.0 };
	os200_list sorted_jobs;

	input = fopen(filename, "r");
	if (input) {
		sorted_jobs = os200_list_new();
		quantum = os200_job_scan(input, sorted_jobs);
		fclose(input);
		result = os200_robin_core(sorted_jobs, quantum);
		os200_list_foreach(
			sorted_jobs,
			os200_job_free_in_list,
			NULL
		);
		os200_list_free(sorted_jobs);
	} else {
		OS200_PRINT("%s: %s", filename, strerror(errno));
	}
	return result;
}

os200_result os200_robin_core(os200_list sorted_jobs, double quantum) {
	double now = 0.0;
	size_t completed_jobs = 0;
	size_t total_jobs = sorted_jobs->count;
	os200_list queue;
	os200_result result = { OS200_SCHEDULER_ROBIN, 0.0 / 0.0, 0.0 / 0.0 };

	os200_list_node current_node = NULL;
	os200_list_node incoming_node = NULL;
	os200_job current_job = NULL;
	os200_job incoming_job = NULL;

	if (!sorted_jobs->count)
		return result;

	queue = os200_list_new();

	incoming_node = sorted_jobs->head;
	incoming_job = incoming_node->data;

	while (completed_jobs < total_jobs) {
		current_node = NULL;
		current_job = NULL;
		if (queue->head) {
			/* there exists a job that is ready to run */
			current_node = os200_list_remove(
				queue, queue->head);
			current_job = current_node->data;
			if (current_job->remaining > quantum) {
				OS200_DEBUG(
					"%f: run [%f %f] for %f", now,
					current_job->arrival,
					current_job->duration,
					quantum
				);
				/* previously requeued here */
				current_job->remaining -= quantum;
				now += quantum;
			} else {
				OS200_DEBUG(
					"%f: run [%f %f] for %f", now,
					current_job->arrival,
					current_job->duration,
					current_job->remaining
				);
				now += current_job->remaining;
				current_job->remaining = 0.0;
				current_job->completion = now;
				completed_jobs++;
				OS200_DEBUG(
					"%f: end [%f %f]", now,
					current_job->arrival,
					current_job->duration
				);
				current_job = NULL;
			}
			os200_list_node_free(current_node);
		} else {
			/* fast forward in time to next arrival */
			if (incoming_node)
				now = incoming_job->arrival;
		}
		while (incoming_node && incoming_job->arrival <= now) {
			OS200_DEBUG(
				"%f: ready [%f %f]", now,
				incoming_job->arrival,
				incoming_job->duration
			);
			os200_list_insert_after(
				queue,
				incoming_job,
				queue->tail
			);
			incoming_node = incoming_node->next;
			if (incoming_node)
				incoming_job = incoming_node->data;
		}
		if (current_job) {
			os200_list_insert_after(
				queue,
				current_job,
				queue->tail
			);
		}
		OS200_DEBUG(
			"%f: completed %zu of %zu",
			now, completed_jobs, total_jobs
		);
	}
	os200_list_free(queue);
	result = os200_result_get(sorted_jobs);
	result.scheduler = OS200_SCHEDULER_ROBIN;
	return result;
}
