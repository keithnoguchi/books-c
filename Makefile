# SPDX-License-Identifier: GPL-2.0
BOOKS	:= algorithms
BOOKS	+= bpf
all:
%:
	@for book in $(BOOKS); do if ! make -C $${book} $*; then exit 1; fi; done
