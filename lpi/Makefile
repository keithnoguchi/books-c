# SPDX-Licence-Identifier: GPL-2.0
CHAPS	:= ch04 ch05 ch06 ch07
all:
%:
	@for chapter in $(CHAPS); do if ! make -C $${chapter} $*; then exit 1; fi done
