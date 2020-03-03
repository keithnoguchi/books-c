# SPDX-License-Identifier: GPL-2.0
PROG	:= drop_tcp
BCCPROG	:= drop_tcp_count

.PHONY: all clean run $(BCCPROG)
all: $(PROG)
$(PROG):
	clang -O2 -target bpf -c $@.c -o $@.o
run: $(BCCPROG)
$(BCCPROG):
	@sudo python $@.py
clean:
	$(RM) *.o
