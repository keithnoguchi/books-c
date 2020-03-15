/* SPDX-License-Identifier: GPL-2.0 */
#include <stdlib.h>
#include <string.h>

#include "dlist.h"

int dlist_init(struct list *list, void (*dtor)(void *data))
{
	memset(list, 0, sizeof(*list));
	list->dtor = dtor;
	list->size = 0;
	return 0;
}

void dlist_destroy(struct list *list)
{
	struct node *node, *next;

	for (node = list->head; node; node = next) {
		next = node->next;
		if (list->dtor)
			list->dtor((void *)node->data);
		free(node);
	}
	memset(list, 0, sizeof(*list));
}

int dlist_ins_next(struct list *list, struct node *node, const void *data)
{
	struct node *new, **next, **prev;

	new = malloc(sizeof(*new));
	if (!new)
		return -1;
	memset(new, 0, sizeof(*new));
	new->next = new->prev = NULL;
	new->data = data;

	if (node)
		next = &node->next;
	else
		next = &list->head;
	if (*next)
		prev = &(*next)->prev;
	else
		prev = &list->tail;
	new->next = *next;
	new->prev = *prev;
	*next = new;
	*prev = new;
	list->size++;
	return 0;
}

int dlist_rem_next(struct list *list, struct node *node, void **data)
{
	struct node *rem, **next, **prev;

	if (node)
		next = &node->next;
	else
		next = &list->head;
	rem = *next;
	if (!rem)
		return -1;
	if (rem->next)
		prev = &rem->next->prev;
	else
		prev = &list->tail;
	*next = rem->next;
	*prev = rem->prev;
	*data = (void *)rem->data;
	free(rem);
	return 0;
}
