# SPDX-Licence-Identifier: GPL-2.0
CHAPS	:= ch04
CHAPS	+= ch05
all:
%:
	@for chapter in $(CHAPS); do if ! make -C $${chapter} $*; then exit 1; fi done
