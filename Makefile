# SPDX-License-Identifier: GPL-2.0
BOOKS	:= bpf

.PHONY:	all build clean
all:	build
build:
	@for book in $(BOOKS); do make -C $${book}; done
clean:
	@for book in $(BOOKS); do make -C $${book} clean; done
