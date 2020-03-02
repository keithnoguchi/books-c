# SPDX-License-Identifier: GPL-2.0
CHS	:= ch07

.PHONY:	all build clean
all:	build
build:
	@for chapter in $(CHS); do make -C $${chapter}; done
clean:
	@for chapter in $(CHS); do make -C $${chapter} clean; done
