/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>

#include "list.h"

int list_init(struct list *list, void (*dtor)(void *data))
{
	if (!list)
		return -1;
	memset(list, 0, sizeof(*list));
	list->dtor = dtor;
	return 0;
}

void list_destroy(struct list *list)
{
	struct node *node, *next;

	for (node = list->head; node; node = next) {
		next = node->next;
		if (node->data && list->dtor)
			list->dtor((void *)node->data);
		free(node);
		list->size--;
	}
}

int list_ins_next(struct list *list, struct node *node, const void *data)
{
	struct node **next;
	struct node *new;

	new = malloc(sizeof(*new));
	if (!new)
		return -1;
	memset(new, 0, sizeof(*new));
	new->data = data;
	if (node) {
		next = &node->next;
		if (node == list->tail)
			list->tail = new;
	} else {
		next = &list->head;
		if (!list->tail)
			list->tail = new;
	}
	new->next = *next;
	*next = new;
	list->size++;
	return 0;
}

int list_rem_next(struct list *list, struct node *node, void **data)
{
	struct node **next;
	struct node *rem;

	if (node) {
		next = &node->next;
		if (*next == list->tail)
			list->tail = node;
	} else
		next = &list->head;
	rem = *next;
	*next = rem->next;
	*data = (void *)rem->data;
	free(rem);
	list->size--;
	return 0;
}
