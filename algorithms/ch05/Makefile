# SPDX-License-Identifier: GPL-2.0
TEST	:= list_test
TESTOBJ	:= $(patsubst %,%.o,$(TEST))
OBJ	:= $(patsubst %_test,%.o,$(TEST))
LIB	:= liba.a
CFLAGS	+= -Wall
CFLAGS	+= -I.
CFLAGS	+= -g
.PHONY: test clean
all: $(LIB)
$(LIB): $(OBJ)
	$(AR) rcv $@ $^
%.o: %.c %.h
	$(CC) $(CFLAGS) -c $*.c -o $@
test: $(TEST)
	@for test in $^; do if ./$${test}; \
		then printf "%s:\tPASS\n" $${test}; \
		else printf "%s:\tFAIL\n" $${test}; exit 1; \
	fi; done
$(TEST): $(TESTOBJ) $(LIB)
	$(CC) $(CFLAGS) -o $@ $@.o $(LIB)
%_test.o: %_test.c %.h
	$(CC) $(CFLAGS) -c $*_test.c -o $@
clean:
	$(RM) $(OBJ) $(LIB) $(PROG)
