firstrule:all

FLEX_SOURCES=avmc_flex.c avmc_bison.c 
FLEX_INCLUDES=avmc_bison.h
FLEX_OTHERS=avmc.output

C_SOURCES=avmc_main.c avmc_ops.c
CFLAGS+= -g -I../avmm -I../avmlib #--save-temps

SOURCES=$(C_SOURCES) $(FLEX_SOURCES)
OBJS=$(SOURCES:%.c=%.o)
PROG=avmc

LIBS=-L../avmlib -lavm -ll

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
	./avmc ../examples/*.avma
