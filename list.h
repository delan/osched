#ifndef OS200_LIST_H
#define OS200_LIST_H

typedef struct os200_list_node {
	void *data;
	os200_list_node prev;
	os200_list_node next;
} *os200_list_node;

typedef struct os200_list {
	size_t count;
	os200_list_node head;
	os200_list_node tail;
} *os200_list;

typedef void (*os200_list_iterator)(void *data, void *extra);

typedef int (*os200_list_comparator)(void *a, void *b);

os200_list_node os200_list_node_new(void *data);

void os200_list_node_free(os200_list_node node);

os200_list os200_list_new(void);

void os200_list_free(os200_list list);

void os200_list_insert_last(os200_list list, void *data);

void os200_list_insert_sorted(os200_list list, void *data,
	os200_list_comparator comparator);

os200_list_node os200_list_remove_node(os200_list list, os200_list_node node);

void os200_list_foreach(os200_list list, os200_list_iterator iterator);

#endif
