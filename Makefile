SHELL := bash

CXX = x86_64-linux-gnu-g++-16

PROJECTS = dumptype

.PHONY: all clean re
all clean re:
	for project in $(PROJECTS); do      \
		make CXX=$(CXX) -C dumptype $@;  \
	done
