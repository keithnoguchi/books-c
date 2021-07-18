/* SPDX-License-Identifier: GPL-2.0 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void dump_time_struct_bytes(const struct tm *const timep, size_t size)
{
	unsigned char *p = (unsigned char *)timep;
	int i;

	printf("bytes of struct located at %p\n", timep);
	p = (unsigned char *)timep;

	for (i = 0; i < size; i++) {
		printf("%02x ", *p++);
		if (i%16 == 15)
			printf("\n");
	}
	printf("\n");
}

int main(void)
{
	int hour, minute, second, i, *intp;
	struct tm current_time, *timep;
	time_t now;

	/* second from unix spoch */
	now = time(NULL);
	printf("time() - seconds since epoch: %ld\n", now);
	
	/* convert it to the local time */
	timep = localtime_r(&now, &current_time);
	if (!timep)
		goto err;

	hour = current_time.tm_hour;
	minute = timep->tm_min;
	second = *((int *)&current_time);

	printf("localtime_r(): Current time is %02d:%02d:%02d\n\n",
	       hour, minute, second);

	dump_time_struct_bytes(timep, sizeof(current_time));
	intp = (int *)timep;
	for (i = 0; i < 3; i++) {
		printf("intp @ %p: %d\n", intp, *intp);
		intp++;
	}
	exit(EXIT_SUCCESS);
err:
	exit(EXIT_FAILURE);
}
