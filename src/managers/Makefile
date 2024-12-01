CC=g++

CFLAGS= -std=c++17 -Wall -g

# source files
SRC=$(wildcard src/managers/*.cpp)

# object files (.o)
OBJ=$(patsubst src/managers/%.cpp,obj/%.o,$(SRC))


bin/exe: $(OBJ)
	$(CC) $^ -o $@
obj/%.o: src/managers/%.cpp src/managers/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm obj/* obj/style/* obj/style/tests/* bin/*
