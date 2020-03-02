# SPDX-License-Identifier: GPL-2.0
PROG	:= drop_tcp

all: $(PROG)
$(PROG):
	clang -O2 -target bpf -c $@.c -o $@.o
clean:
	$(RM) *.o
