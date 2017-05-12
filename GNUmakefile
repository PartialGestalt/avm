firstrule: all


SUBDIRS=feplib fepc #fepvm


.DEFAULT:
	@(for subdir in $(SUBDIRS); do make -C $$subdir $@ || exit; done)
