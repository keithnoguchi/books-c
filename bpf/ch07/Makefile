# SPDX-License-Identifier: GPL-2.0
PROG	:= drop_tcp
BCCPROG	:= drop_tcp_count
BCCTEST	:= drop_tcp_or_rewrite_test

.PHONY: all run test clean
all: $(PROG) test
$(PROG):
	clang -O2 -target bpf -c $@.c -o $@.o
.PHONY: $(BCCPROG)
run: $(BCCPROG)
$(BCCPROG):
	@sudo python $@.py
.PHONY: $(BCCTEST)
test: $(BCCTEST)
$(BCCTEST):
	sudo python $@.py
clean:
	$(RM) *.o
