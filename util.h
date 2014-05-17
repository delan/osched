#ifndef OS200_UTIL_H
#define OS200_UTIL_H

#include <pthread.h>

#define OS200_LOCKED_GLOBAL(NAME) \
	pthread_mutex_t NAME ## _mutex

#define OS200_LOCKED_EXTERN(NAME) \
	extern pthread_mutex_t NAME ## _mutex

OS200_LOCKED_EXTERN(stderr);

#define OS200_DEBUG(...) do { \
	OS200_LOCK(stderr); \
	fprintf(stderr, "%s: ", __func__); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr); \
	OS200_UNLOCK(stderr); \
} while (0)

#define OS200_SYNCHRONISED_GLOBAL(NAME) \
	int NAME ## _ready = 0; \
	pthread_cond_t NAME ## _cond = PTHREAD_COND_INITIALIZER; \
	OS200_LOCKED_GLOBAL(NAME)

#define OS200_NEW_SYNCHRONISED_GLOBAL(TYPE, NAME) \
	TYPE NAME; \
	OS200_SYNCHRONISED_GLOBAL(NAME)

#define OS200_UNUSED(NAME) do { \
	((void) sizeof(NAME)); \
} while (0)

#define OS200_WAIT(NAME) do { \
	os200_wait_real( \
		& NAME ## _ready, \
		& NAME ## _mutex, \
		& NAME ## _cond \
	); \
} while (0)

#define OS200_SIGNAL(NAME) do { \
	os200_signal_real( \
		& NAME ## _ready, \
		& NAME ## _mutex, \
		& NAME ## _cond \
	); \
} while (0)

#define OS200_LOCK(NAME) do { \
	pthread_mutex_lock(& NAME ## _mutex); \
} while (0)

#define OS200_UNLOCK(NAME) do { \
	pthread_mutex_unlock(& NAME ## _mutex); \
} while (0)

#define OS200_RESET(NAME) do { \
	NAME ## _ready = 0; \
} while (0)

#define OS200_UNLOCK_RESET(NAME) do { \
	OS200_UNLOCK(NAME); \
	OS200_RESET(NAME); \
} while (0)

typedef enum os200_scheduler {
	OS200_SCHEDULER_ROBIN,
	OS200_SCHEDULER_SJF
} os200_scheduler;

char *os200_read_line(const char *prompt);

char *os200_strdup(const char *s);

int os200_wait_real(
	int *state_variable,
	pthread_mutex_t *mutex,
	pthread_cond_t *condition_variable
);

int os200_signal_real(
	int *state_variable,
	pthread_mutex_t *mutex,
	pthread_cond_t *condition_variable
);

#endif
