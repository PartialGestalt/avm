firstrule:all

FLEX_SOURCES=fepc_flex.c fepc_bison.c fepc_bison.h
SOURCES=$(C_SOURCES) $(FLEX_SOURCES)
OBJS=$(SOURCES:%.c=%.o)
PROG=fepc

LIBS=-ll



fresh:: clean all 

clean::
	rm -rf $(OBJS) $(PROG) $(FLEX_SOURCES)

all: $(PROG)

%_flex.c: %.l
	flex -o $@ $^


%_bison.c: %.y
	bison -d -o $@ $^

$(OBJS): $(SOURCES)

$(PROG): $(OBJS)
	cc -o $(PROG) $(OBJS) $(LIBS)
