# SPDX-License-Identifier: GPL-2.0
PROG	:= copy
CFLAGS	+= -Wall
CFLAGS	+= -I.
CFLAGS	+= -g
.PHONY: test clean
all: $(PROG)
%: %.o
	$(CC) $(CFLAGS) $*.c -o $@
clean:
	@$(RM) $(PROG)
