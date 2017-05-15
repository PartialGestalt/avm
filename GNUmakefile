firstrule: all


SUBDIRS=avmlib avmc avmm

cscope: cscope.out

cscope.out: cscope.files
	cscope -bi $^

cscope.files::
	find . -type f -name '*.[ch]' -print > $@

CSCOPE_FILES=cscope.files cscope.out

CLEANFILES=$(CSCOPE_FILES)

DESCEND=@(for subdir in $(SUBDIRS); do make -C $$subdir $@ || exit; done)


clean::
	rm -rf $(CLEANFILES) 2>/dev/null
	$(DESCEND)

.DEFAULT:
	$(DESCEND)
