# SPDX-License-Identifier: GPL-2.0
SUDO	?= sudo
CLANG	?= clang
PYTHON	?= python
PROG	:= drop_tcp
BCCPROG	:= drop_tcp_count
BCCTEST	:= drop_tcp_or_rewrite_test

.PHONY: run test clean
all: $(PROG)
$(PROG):
	$(CLANG) -O2 -target bpf -c $@.c -o $@.o
.PHONY: $(BCCPROG)
run: $(BCCPROG)
$(BCCPROG):
	@$(SUDO) $(PYTHON) $@.py
.PHONY: $(BCCTEST)
test: $(BCCTEST)
$(BCCTEST):
	$(SUDO) $(PYTHON) $@.py
clean:
	@$(RM) *.o
