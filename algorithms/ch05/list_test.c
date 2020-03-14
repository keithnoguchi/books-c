/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "list.h"

struct test {
	const char	*const name;
	const int	data[10];
	const int	want[10];
	int		(*cmp)(const void *k1, const void *k2);
	void		(*dtor)(void *data);
	size_t		size;
};

static int intcmp(const void *k1, const void *k2)
{
	int *d1 = (int *)k1;
	int *d2 = (int *)k2;
	return *d1 - *d2;
}

int main()
{
	const struct test *t, tests[] = {
		{
			.name	= "zero data",
			.data	= {},
			.want	= {},
			.cmp	= NULL,
			.dtor	= NULL,
			.size	= 0,
		},
		{
			.name	= "one integer",
			.data	= {1},
			.want	= {1},
			.cmp	= intcmp,
			.dtor	= NULL,
			.size	= 1,
		},
		{
			.name	= "five integers",
			.data	= {1, 2, 3, 4, 5},
			.want	= {1, 2, 3, 4, 5},
			.cmp	= intcmp,
			.dtor	= NULL,
			.size	= 5,
		},
		{ .name = NULL },
	};
	int fail = 0;
	for (t = tests; t->name; t++) {
		struct node *node;
		struct list l;
		int i, ret;

		ret = list_init(&l, t->dtor);
		if (ret) {
			fprintf(stderr, "%s: %s\n", t->name, strerror(errno));
			goto fail;
		}
		for (i = 0; i < t->size; i++)
			ret = list_ins_next(&l, l.tail, (void *)&t->data[i]);
		if (l.size != t->size) {
			fprintf(stderr, "%s: unexpected size:\n\t- want: %ld\n\t-  got: %ld\n",
				t->name, t->size, l.size);
			goto fail;
		}
		for (i = 0, node = l.head; node; i++, node = node->next) {
			ret = t->cmp(node->data, &t->want[i]);
			if (ret) {
				fprintf(stderr, "%s: unexpected %dth value\n", t->name, i);
				goto fail;
			}
		}
		for (i = 0; i < t->size; i++) {
			void *got;
			ret = list_rem_next(&l, NULL, &got);
			if (ret)
				goto fail;
			ret = t->cmp(got, &t->want[i]);
			if (ret) {
				fprintf(stderr, "%s: unexpected %dth value\n", t->name, i);
				goto fail;
			}
		}
		if (l.size) {
			fprintf(stderr, "%s: unexpected final size:\n\t- want: 0\n\t-  got: %ld\n",
				t->name, l.size);
			goto fail;
		}
		list_destroy(&l);
		continue;
fail:
		fail++;
	}
	if (fail)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
