# SPDX-License-Identifier: GPL-2.0
BOOKS	:= bpf
all:
%:
	@for book in $(BOOKS); do make -C $${book} $*; done
