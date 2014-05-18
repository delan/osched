#ifndef OS200_UTIL_H
#define OS200_UTIL_H

#include <pthread.h>

/* declare a new mutex lock */

#define OS200_LOCK_NEW(NAME)                                           \
	pthread_mutex_t NAME ## _mutex = PTHREAD_MUTEX_INITIALIZER

/* declare the existence of a global mutex lock */

#define OS200_LOCK_EXTERN(NAME)                                        \
	extern pthread_mutex_t NAME ## _mutex

/* acquire a mutex lock */

#define OS200_LOCK_ACQUIRE(NAME) do {                                  \
	OS200_CHECK(pthread_mutex_lock, & NAME ## _mutex);             \
} while (0)

/* release a mutex lock */

#define OS200_LOCK_RELEASE(NAME) do {                                  \
	OS200_CHECK(pthread_mutex_unlock, & NAME ## _mutex);           \
} while (0)

/* declare a new synchroniser */

#define OS200_SYNC_NEW(NAME)                                           \
	int NAME ## _state = 0;                                        \
	pthread_cond_t NAME ## _cond = PTHREAD_COND_INITIALIZER;       \
	OS200_LOCK_NEW(NAME)

/* reset the state of a synchroniser */

#define OS200_SYNC_RESET(NAME) do {                                    \
	NAME ## _state = 0;                                            \
} while (0)

/* acquire a synchroniser's mutex lock when its state is not set */

#define OS200_SYNC_LOCK(NAME) do {                                     \
	OS200_LOCK_ACQUIRE(NAME);                                      \
	while (NAME ## _state) {                                       \
		OS200_LOCK_RELEASE(NAME);                              \
		OS200_LOCK_ACQUIRE(NAME);                              \
	}                                                              \
} while (0)

/* wait until a synchroniser is signalled, acquiring its mutex lock */

#define OS200_SYNC_WAIT(NAME) do {                                     \
	while (NAME ## _state == 0)                                    \
		OS200_CHECK(                                           \
			pthread_cond_wait,                             \
			& NAME ## _cond,                               \
			& NAME ## _mutex                               \
		);                                                     \
} while (0)

/* signal the thread listening on a synchroniser */

#define OS200_SYNC_SIGNAL(NAME) do {                                   \
	OS200_SYNC_LOCK(NAME);                                         \
	NAME ## _state = 1;                                            \
	OS200_CHECK(pthread_cond_signal, & NAME ## _cond);             \
	OS200_LOCK_RELEASE(NAME);                                      \
} while (0)

/* set the value of a synchroniser's shared data, then signal */

#define OS200_SYNC_SET_SIGNAL(NAME, VALUE) do {                        \
	OS200_SYNC_LOCK(NAME);                                         \
	NAME ## _state = 1;                                            \
	NAME = VALUE;                                                  \
	OS200_CHECK(pthread_cond_signal, & NAME ## _cond);             \
	OS200_LOCK_RELEASE(NAME);                                      \
} while (0)

/* generic utilities */

OS200_LOCK_EXTERN(stderr);

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

#define OS200_CHECK(NAME, ...) do { \
	int retval = NAME(__VA_ARGS__); \
	if (retval) \
		OS200_PRINT("%s: %s", #NAME, strerror(retval)); \
} while (0)

#define OS200_UNUSED(NAME) do { \
	((void) sizeof(NAME)); \
} while (0)

char *os200_read_line(const char *prompt);

char *os200_strdup(const char *s);

#endif
