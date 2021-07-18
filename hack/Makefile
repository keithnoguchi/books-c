# SPDX-Licence-Identifier: GPL-2.0
CHS	:= ch02
all:
%:
	@for chapter in $(CHS); do if ! make -C $${chapter} $*; then exit 1; fi done
