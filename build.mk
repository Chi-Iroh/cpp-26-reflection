SHELL := bash

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