#ifndef OS200_UTIL_H
#define OS200_UTIL_H

#define OS200_DEBUG(...) do { \
	fprintf(stderr, "%s: ", __func__); \
	fprintf(stderr, __VA_ARGS__); \
	fputc('\n', stderr); \
} while (0)

char *os200_read_line(const char *prompt);

#endif
