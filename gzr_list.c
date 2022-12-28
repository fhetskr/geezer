#include "include/gzr_list.h"

glist *new_glist() {
	glist *new_list = malloc(sizeof(glist));
	// check for allocation errors
	if (new_list == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory to list\n");
		exit(1);
	}
	new_list->head = NULL;
	new_list->tail = NULL;
	new_list->len = 0;
	
	return new_list;
}

struct gzr_node *new_node(gvalue *value) {
	struct gzr_node *new_node = malloc(sizeof(struct gzr_node));
	if (new_node == NULL) {
		fprintf(stderr,
			"Fatal error encountered while allocating memory to list node\n");
		exit(1);
	}
	new_node->contents = value;
	value->ref_count++;
	new_node->next = NULL;
	new_node->last = NULL;
	
	return new_node;
}

void destroy_node(struct gzr_node *node) {
	deref(node->contents);
	if (node->next)
		node->next->last = node->last;
	if (node->last)
		node->last->next = node->next;
	free(node);
}

int ginsert(glist *list, gvalue *new_item, int index) {
	struct gzr_node *node = list->head;
	if (node == NULL || index > list->len - 1 || index < -(list->len))
		return -1;
	if (index >= 0) {
		for (int i = 0; i < index; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (int i = -1; i > index; i--)
			node = node->last;
	}
	struct gzr_node *new = new_node(new_item);
	if (list->head == node)
		list->head = new;
	if (list->tail == node)
		list->tail = new;
	new->last = node->last;
	node->last = new;
	new->next = node;
}

void gappend(glist *list, gvalue *new_value) {
	if (list->head) {
		list->tail->next = new_node(new_value);
		list->tail->next->last = list->tail;
		list->tail = list->tail->next;
	} else {
		list->head = new_node(new_value);
		list->tail = list->head;
	}
	list->len++;
}

gvalue *gget(glist *list, int index) {
	struct gzr_node *node = list->head;
	if (node == NULL || index > list->len - 1 || index < -(list->len))
		return new_gvalue();
	if (index >= 0) {
		for (int i = 0; i < index; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (int i = -1; i > index; i--)
			node = node->last;
	}
	return node->contents;
}

int gremove(glist *list, int index) {
	struct gzr_node *node = list->head;
	if (node == NULL || index > list->len - 1 || index < -(list->len))
		return -1;
	if (index >= 0) {
		for (int i = 0; i < index; i++)
			node = node->next;
	} else {
		node = list->tail;
		for (int i = -1; i > index; i--)
			node = node->last;
	}
	if (node == list->head)
		list->head = node->next;
	if (node == list->tail)
		list->tail = node->last;
	destroy_node(node);
	return 0;
}

int gfind(glist *list, gvalue *gval) {
	struct gzr_node *node = list->head;
	if (node == NULL)
		return -1;
	int i = 0;
	while (node) {
		if (node->contents == gval)
			return i;
		i++;
		node = node->next;
	}
}

void destroy_list(glist *list) {
	while (list->head)
		gremove(list, 0);
	free(list);
}