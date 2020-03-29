/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _DLIST_H
#define _DLIST_H

struct list {
	struct node	*head;
	struct node	*tail;
	size_t		size;
	void		(*dtor)(void *data);
};

struct node {
	struct node	*next;
	struct node	*prev;
	const void	*data;
};

int dlist_init(struct list *list, void (*dtor)(void *data));
void dlist_destroy(struct list *list);
int dlist_ins_next(struct list *list, struct node *node, const void *data);
int dlist_rem_next(struct list *list, struct node *node, void **data);

#endif /* _DLIST_H */
