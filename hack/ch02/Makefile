# SPDX-License-Identifier: GPL-2.0
PROG	:= loop array size pointer input typecast pointer_type cmdline
PROG	+= convert scope static frame segment heap note uid
PROG	+= notetaker notesearch time functor rand game
# multiuser commands.
SETSID	:= notetaker notesearch game
CFLAGS	+= -Wall -Werror
CFLAGS	+= -I.
CFLAGS	+= -g
.PHONY: test clean setsid
all: $(PROG) setsid
setsid:
	for prog in $(SETSID); do sudo chown root $${prog} && sudo chmod u+s $${prog}; done
%: %.o
	$(CC) $(CFLAGS) $*.c -o $@
clean:
	@$(RM) $(PROG)
