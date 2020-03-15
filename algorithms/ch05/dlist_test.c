/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "dlist.h"

struct test {
	const char	*const name;
	const int	data[10];
	const int	want[10];
	size_t		size;
	void		(*dtor)(void *data);
};

static int cmp(const void *k1, const void *k2)
{
	const int *d1 = (const int *)k1;
	const int *d2 = (const int *)k2;
	return *d1 - *d2;
}

static int test(const struct test *const t)
{
	struct node *node;
	struct list list;
	const int *want;
	int i, ret;

	ret = dlist_init(&list, t->dtor);
	if (ret) {
		fprintf(stderr, "%s: %s\n", t->name, strerror(errno));
		goto fail;
	}
	for (i = 0; i < t->size; i++) {
		ret = dlist_ins_next(&list, list.tail, (void *)&t->data[i]);
		if (ret) {
			fprintf(stderr, "%s: dlist_ins_next() error", t->name);
			goto fail;
		}
	}
	for (node = list.head, want = t->want; node; node = node->next, want++)
		if (cmp(node->data, want)) {
			fprintf(stderr, "%s: unexpected %dth data\n", t->name, i);
			goto fail;
		}
	if (list.size != t->size) {
		fprintf(stderr, "%s: unexpected inserted size:\n\t- want: %ld\n\t-  got: %ld\n",
			t->name, t->size, list.size);
		goto fail;
	}
	for (i = 0; i < t->size; i++) {
		int *got;
		ret = dlist_rem_next(&list, NULL, (void **)&got);
		if (ret) {
			fprintf(stderr, "%s: dlist_rem_next() error", t->name);
			goto fail;
		}
		if (cmp(got, &t->want[i])) {
			fprintf(stderr, "%s: unexpected %dth data:\n\t- want: %d\n\t:-  got: %d\n",
				t->name, i, t->want[i], *got);
			goto fail;
		}
	}
	dlist_destroy(&list);
	if (list.size) {
		fprintf(stderr, "%s: unexpected destroyed size:\n\t- want: 0\n\t-  got: %ld\n",
			t->name, list.size);
		goto fail;
	}
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
			.size	= 0,
			.dtor	= NULL,
		},
		{
			.name	= "single data",
			.data	= {1},
			.want	= {1},
			.size	= 1,
			.dtor	= NULL,
		},
		{.name = NULL},
	};
	int fail = 0;

	for (t = tests; t->name; t++)
		if (test(t))
			fail++;
	if (fail)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
