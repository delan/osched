#ifndef OS200_UTIL_H
#define OS200_UTIL_H

#include <pthread.h>

/* shorthand declarations for locks and condition variables */

#define OS200_LOCKED_GLOBAL(NAME) \
	pthread_mutex_t NAME ## _mutex = PTHREAD_MUTEX_INITIALIZER

#define OS200_LOCKED_EXTERN(NAME) \
	extern pthread_mutex_t NAME ## _mutex

#define OS200_SYNCHRONISED_GLOBAL(NAME) \
	int NAME ## _ready = 0; \
	pthread_cond_t NAME ## _cond = PTHREAD_COND_INITIALIZER; \
	OS200_LOCKED_GLOBAL(NAME)

#define OS200_NEW_SYNCHRONISED_GLOBAL(TYPE, NAME) \
	TYPE NAME; \
	OS200_SYNCHRONISED_GLOBAL(NAME)

/* basic locking and signalling patterns */

#define OS200_WAIT(NAME) do { \
	OS200_CHECK( \
		pthread_mutex_lock, \
		& NAME ## _mutex \
	); \
	while (NAME ## _ready == 0) \
		OS200_CHECK( \
			pthread_cond_wait, \
			& NAME ## _cond, \
			& NAME ## _mutex \
		); \
} while (0)

#define OS200_SIGNAL(NAME) do { \
	NAME ## _ready = 1; \
	OS200_CHECK(pthread_cond_broadcast, & NAME ## _cond); \
	OS200_CHECK(pthread_mutex_unlock, & NAME ## _mutex); \
} while (0)

#define OS200_LOCK(NAME) do { \
	OS200_CHECK(pthread_mutex_lock, & NAME ## _mutex); \
} while (0)

#define OS200_UNLOCK(NAME) do { \
	OS200_CHECK(pthread_mutex_unlock, & NAME ## _mutex); \
} while (0)

#define OS200_RESET(NAME) do { \
	NAME ## _ready = 0; \
	OS200_UNLOCK(NAME); \
} while (0)

/* thread-safe printf that includes source filename, line and function */

OS200_LOCKED_EXTERN(stderr);

#define OS200_PRINT(FORMAT, ...) do { \
	pthread_mutex_lock(&stderr_mutex); \
	fprintf( \
		stderr, \
		"%s:%d: %s: " FORMAT "\n", \
		__FILE__, \
		__LINE__, \
		__func__, \
		__VA_ARGS__ \
	); \
	pthread_mutex_unlock(&stderr_mutex); \
} while (0)

#ifdef DEBUG
	#define OS200_DEBUG OS200_PRINT
#else
	#define OS200_DEBUG(...) do { \
	} while (0)
#endif

/* call a library function, check its return value and report errors */

#define OS200_CHECK(NAME, ...) do { \
	int retval = NAME(__VA_ARGS__); \
	if (retval) \
		OS200_PRINT("%s: %s", #NAME, strerror(retval)); \
} while (0)

/* simple scheduler type identifier */

typedef enum os200_scheduler {
	OS200_SCHEDULER_ROBIN,
	OS200_SCHEDULER_SJF
} os200_scheduler;

/* generic utility functions */

#define OS200_UNUSED(NAME) do { \
	((void) sizeof(NAME)); \
} while (0)

char *os200_read_line(const char *prompt);

char *os200_strdup(const char *s);

#endif
