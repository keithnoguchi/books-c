# SPDX-License-Identifier: GPL-2.0
PROG	:= loop
CFLAGS	+= -Wall -Werror
CFLAGS	+= -I.
CFLAGS	+= -g
.PHONY: test clean
all: $(PROG)
%: %.o
	$(CC) $(CFLAGS) $*.c -o $@
clean:
	@$(RM) $(PROG)
