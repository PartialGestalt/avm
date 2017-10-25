firstrule: all

# Unless we're forcing GCC, use clang

ifeq ($(CC),cc)
CC = clang
endif


LIB_TOKEN=avm
LIB_TARGET=lib$(LIB_TOKEN).a
LIB_SRCS=$(wildcard avmlib_*.c)
LIB_OBJS=$(LIB_SRCS:.c=.o)

LIB_CFLAGS=-fPIC -I../avmm -I../avmlib
LIB_LDFLAGS=

DEBUG_CFLAGS=-g

CFLAGS+=$(LIB_CFLAGS) $(DEBUG_CFLAGS)

CLEANFILES=$(LIB_TARGET) $(LIB_OBJS)

all: $(LIB_TARGET)


$(LIB_TARGET): $(LIB_OBJS)
	ar -rc $@ $^

clean::
	rm -rf $(CLEANFILES) 2>/dev/null


fresh: clean all

.DEFAULT:
	@echo No rule here to make $@
