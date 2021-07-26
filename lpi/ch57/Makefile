# SPDX-License-Identifier: GPL-2.0
PROG	:= stream-server stream-client
PROG	+= dgram-server dgram-client
CFLAGS	+= -Wall -Werror
CFLAGS	+= -Wextra
CFLAGS	+= -I.
CFLAGS	+= -g
CFLAGS	+= -O
.PHONY: test clean
all: $(PROG)
%: %.o
	$(CC) $(CFLAGS) $*.c -o $@
clean:
	@$(RM) $(PROG)
