#include "util.h"

#include <stdio.h>
#include <stdlib.h>

char *os200_read_line(const char *prompt) {
	int temp;
	size_t count = 0;
	size_t capacity = 16;
	char *result = malloc(capacity);
	printf("%s: ", prompt);
	temp = getchar();
	while (temp != EOF && temp != '\n') {
		result[count++] = temp;
		if (count > capacity - 2) {
			capacity *= 2;
			result = realloc(result, capacity);
		}
		temp = getchar();
	}
	result[count++] = '\0';
	return realloc(result, count);
}
