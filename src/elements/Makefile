CC=g++

CFLAGS= -std=c++17 -Wall -g

# source files
SRC=$(wildcard src/elements/*.cpp)

# object files (.o)
OBJ=$(patsubst src/elements/%.cpp,obj/%.o,$(SRC))


bin/exe: $(OBJ)
	$(CC) $^ -o $@
obj/%.o: src/elements/%.cpp src/elements/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm obj/* obj/style/* obj/style/tests/* bin/*


