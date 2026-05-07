SHELL := bash

AR = x86_64-linux-gnu-gcc-ar-16
CXX = x86_64-linux-gnu-g++-16

# https://stackoverflow.com/a/18258352
rwildcard=$(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))

SRC = $(call rwildcard,src,*.cpp)
OBJ = $(subst src,obj,$(SRC:.cpp=.o))
DEPS = $(subst src,deps,$(SRC:.cpp=.d))

.PHONY: all
all: $(BIN) $(LIB)

$(BIN): $(OBJ)
	$(CXX) $^ $(LDFLAGS) -o $@

$(LIB): $(OBJ)
	$(AR) rc $@ $^

-include $(DEPS)

obj/%.o: src/%.cpp
# creates the subdirectory in obj/ to store the .o
	@mkdir -p $(@D) $(patsubst obj/%,deps/%,$(@D))
	$(CXX) -c -std=c++26 -freflection -MMD -MP -MF $(patsubst %.cpp,%.d,$(subst src,deps,$<)) -I. -I.. $< -o $@

re: clean all

clean:
	rm -rf $(OBJ) $(DEPS) $(BIN)