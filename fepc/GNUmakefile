firstrule:all

FLEX_SOURCES=fepc_flex.c fepc_bison.c 
FLEX_INCLUDES=fepc_bison.h
FLEX_OTHERS=fepc.output

C_SOURCES=fepc_main.c fepc_ops.c
CFLAGS+= -g -I../fepvm -I../feplib #--save-temps

SOURCES=$(C_SOURCES) $(FLEX_SOURCES)
OBJS=$(SOURCES:%.c=%.o)
PROG=fepc

LIBS=-L../feplib -lfep -ll

ALL_INTERMEDIATES=$(wildcard *.s) $(wildcard *.i)


CLEANFILES=$(OBJS) $(PROG) $(FLEX_SOURCES) $(FLEX_INCLUDES) $(FLEX_OTHERS) $(ALL_INTERMEDIATES)

fresh:: clean all 

clean::
	rm -rf $(CLEANFILES)

all: $(PROG)

%_flex.c: %.l
	flex -o $@ $^


%_bison.c: %.y
	bison -d -o $@ $^

$(OBJS): $(SOURCES)

$(PROG): $(OBJS)
	cc -o $(PROG) $(OBJS) $(LIBS)


test: all
	./fepc ../examples/*.fep
