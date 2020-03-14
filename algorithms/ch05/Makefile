# SPDX-License-Identifier: GPL-2.0
PROG	:= list
TEST	:= $(patsubst %,%_test,$(PROG))
CFLAGS	+= -Wall
CFLAGS	+= -I.
.PHONY: clean
all: $(PROG)
$(PROG):
	$(CC) $(CFLAGS) -c $@.c -o $@.o
clean:
	$(RM) *.o
