# SPDX-License-Identifier: GPL-2.0
CHAPS	:= ch07
all:
%:
	@for chap in $(CHAPS); do if ! make -C $${chap} $*; then exit 1; fi; done
