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
	void		(*dtor)(void *data);
	size_t		size;
};

static int cmp(const void *k1, const void *k2)
{
	int *d1 = (int *)k1;
	int *d2 = (int *)k2;
	return *d1 - *d2;
}

static int test(const struct test *const t)
{
	struct node *node;
	struct list list;
	const int *want;
	int i, ret;

	ret = list_init(&list, t->dtor);
	if (ret) {
		fprintf(stderr, "%s: %s\n", t->name, strerror(errno));
		goto fail;
	}
	for (i = 0; i < t->size; i++)
		ret = list_ins_next(&list, list.tail, (void *)&t->data[i]);
	if (list.size != t->size) {
		fprintf(stderr, "%s: unexpected size:\n\t- want: %ld\n\t-  got: %ld\n",
			t->name, t->size, list.size);
		goto fail;
	}
	for (node = list.head, want = t->want; node; node = node->next, want++) {
		ret = cmp(node->data, want);
		if (ret) {
			fprintf(stderr, "%s: unexpected %dth value\n", t->name, i);
			goto fail;
		}
	}
	for (i = 0, want = t->want; i < t->size; i++, want++) {
		void *got;
		ret = list_rem_next(&list, NULL, &got);
		if (ret)
			goto fail;
		if (cmp(got, want)) {
			fprintf(stderr, "%s: unexpected %dth value\n", t->name, i);
			goto fail;
		}
	}
	if (list.size) {
		fprintf(stderr, "%s: unexpected final size:\n\t- want: 0\n\t-  got: %ld\n",
			t->name, list.size);
		goto fail;
	}
	list_destroy(&list);
	return 0;
fail:
	return -1;
}

int main()
{
	const struct test *t, tests[] = {
		{
			.name	= "zero data",
			.data	= {},
			.want	= {},
			.dtor	= NULL,
			.size	= 0,
		},
		{
			.name	= "one integer",
			.data	= {1},
			.want	= {1},
			.dtor	= NULL,
			.size	= 1,
		},
		{
			.name	= "five integers",
			.data	= {1, 2, 3, 4, 5},
			.want	= {1, 2, 3, 4, 5},
			.dtor	= NULL,
			.size	= 5,
		},
		{ .name = NULL },
	};
	int fail = 0;
	for (t = tests; t->name; t++)
		if (test(t))
			fail++;
	if (fail)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
