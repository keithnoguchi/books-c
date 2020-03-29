/* SPDX-License-Identifier: GPL-2.0 */
#ifndef _LIST_H
#define _LIST_H

struct list {
	struct node	*head;
	struct node	*tail;
	size_t		size;
	void		(*dtor)(void *data);
};

struct node {
	struct node	*next;
	const void	*data;
};

int list_init(struct list *list, void (*dtor)(void *data));
void list_destroy(struct list *list);
int list_ins_next(struct list *list, struct node *node, const void *data);
int list_rem_next(struct list *list, struct node *node, void **data);

#endif /* _LIST_H */
