#ifndef GVALUE_H
#define GVALUE_H
#include "gzr_value.h"
#endif

struct gzr_list {
	struct gzr_node *head;
	struct gzr_node *tail;
	int len;
} typedef glist;

struct gzr_node {
	gvalue *contents;
	struct gzr_node *next;
	struct gzr_node *last;
};

glist *new_glist();
struct gzr_node *new_node(gvalue *value);
void destroy_node(struct gzr_node *node);
void destroy_list(glist *list);

int ginsert(glist *list, gvalue *new_item, int index);
void gappend(glist *list, gvalue *new_item);
gvalue *gget(glist *list, int index);
int gremove(glist *list, int index);
int gfind(glist *list, gvalue *value);

//todo:
// slicing