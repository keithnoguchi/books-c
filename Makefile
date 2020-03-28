# SPDX-License-Identifier: GPL-2.0
BOOKS	:= algorithm
BOOKS	+= bpf
BOOKS	+= lpi
all:
%:
	@for book in $(BOOKS); do if ! make -C $${book} $*; then exit 1; fi; done
