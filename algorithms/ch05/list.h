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

#endif /* _LIST_H */
