SHELL := bash

CXX = x86_64-linux-gnu-g++-16

# https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,src,*.cpp)
OBJ = $(subst src,obj,$(SRC:.cpp=.o))
DEPS = $(subst src,deps,$(SRC:.cpp=.d))
BIN = a.out

.PHONY: all
all: $(BIN)

$(BIN): $(OBJ)
	$(CXX) $^ -o $@

-include $(DEPS)

obj/%.o: src/%.cpp
# creates the subdirectory in obj/ to store the .o
	@mkdir -p $(@D) $(patsubst obj/%,deps/%,$(@D))
	$(CXX) -c -std=c++26 -freflection -MMD -MP -MF $(patsubst %.cpp,%.d,$(subst src,deps,$<)) -I. $< -o $@

re: clean all

clean:
	rm -rf $(OBJ) $(DEPS) $(BIN)