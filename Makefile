SHELL := bash

CXX = x86_64-linux-gnu-g++-16

SRC = $(wildcard *.cpp)
OBJ = $(addprefix obj/,$(SRC:.cpp=.o))

.PHONY: all
all: $(OBJ)
	$(CXX) $(OBJ) -o a.out

obj/%.o: %.cpp
	$(CXX) -c -std=c++26 -freflection $^ -o $@

re: clean all

clean:
	rm -rf $(OBJ)