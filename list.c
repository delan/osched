#include "list.h"

#include <stdlib.h>

os200_list_node os200_list_node_new(void *data) {
	os200_list_node node = malloc(sizeof(struct os200_list_node));
	node->data = data;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

void os200_list_node_free(os200_list_node node) {
	free(node);
}

os200_list os200_list_new(void) {
	os200_list list = malloc(sizeof(struct os200_list));
	list->count = 0;
	list->head = NULL;
	list->tail = NULL;
	return list;
}

void os200_list_free(os200_list list) {
	os200_list_node node = list->head;
	while (node) {
		os200_list_node next = node->next;
		os200_list_node_free(node);
		node = next;
	}
	free(list);
}

os200_list_node os200_list_insert_after(os200_list list,
	os200_list_node which, void *data) {
	os200_list_node before = which;
	os200_list_node node = os200_list_node_new(data);
	os200_list_node after = which ? which->next : NULL;
	if (before) {
		before->next = node;
		node->prev = before;
	}
	if (after) {
		node->next = after;
		after->prev = node;
	}
	if (before == NULL) {
		list->head = node;
	}
	if (after == NULL) {
		list->tail = node;
	}
	list->count++;
	return node;
}

os200_list_node os200_list_insert_sorted(os200_list list, void *data,
	os200_list_comparator comparator) {
	os200_list_node cursor = list->head;
	while (cursor && comparator(cursor->data, data) < 0)
		cursor = cursor->next;
	return os200_list_insert_after(list, cursor, data);
}

os200_list_node os200_list_remove(os200_list list,
	os200_list_node node) {
	if (list->head == node)
		list->head = node->next;
	if (list->tail == node)
		list->tail = node->prev;
	node->prev = NULL;
	node->next = NULL;
	return node;
}

void os200_list_foreach(os200_list list,
	os200_list_iterator iterator, void *extra) {
	os200_list_node node = list->head;
	while (node) {
		iterator(node->data, extra);
		node = node->next;
	}
}
