firstrule: all


LIB_TOKEN=fep
LIB_TARGET=lib$(LIB_TOKEN).so
LIB_SRCS=$(wildcard feplib_*.c)
LIB_OBJS=$(LIB_SRCS:.c=.o)

LIB_CFLAGS=
LIB_LDFLAGS=

CFLAGS+=$(LIB_CFLAGS)

CLEANFILES=$(LIB_TARGET) $(LIB_OBJS)

all: $(LIB_TARGET)


$(LIB_TARGET): $(LIB_OBJS)
	gcc -shared -o $@ $^ $(LIB_LDFLAGS)
	



clean::
	rm -rf $(CLEANFILES) 2>/dev/null


fresh: clean all

.DEFAULT:
	@echo No rule here to make $@
