SHELL := bash

# https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,src,*.cpp)
OBJ = $(subst src,obj,$(SRC:.cpp=.o))
DEPS = $(subst src,deps,$(SRC:.cpp=.d))
BIN = dumptype.out

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@

-include $(DEPS)

include ../build.mk
