SHELL := bash

PROJECTS = dumptype argparse

.PHONY: all clean re
all clean re:
	@for project in $(PROJECTS); do      \
		make -C $$project $@;  \
	done
