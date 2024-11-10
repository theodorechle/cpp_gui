CC=g++

CFLAGS= -std=c++17 -Wall -g

# source files
SRC=$(wildcard src/*.cpp)

# object files (.o)
OBJ=$(patsubst src/%.cpp,obj/%.o,$(SRC))

# source files for style 
SRC_STYLE=$(wildcard src/style/*.cpp)

# object files for style (.o)
OBJ_STYLE=$(patsubst src/style/%.cpp,obj/style/%.o,$(SRC_STYLE))


bin/exe: $(OBJ)
	$(CC) $^ -o $@
bin/style: $(OBJ_STYLE)
	$(CC) $^ -o $@
bin/style-tests: obj/style/tests/tests.o $(filter-out obj/style/main.o, $(OBJ_STYLE))
	$(CC) $^ -o $@

obj/main.o : src/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@
obj/%.o: src/%.cpp src/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

obj/style/main.o : src/style/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@
obj/style/%.o: src/style/%.cpp src/style/%.hpp
	$(CC) $(CFLAGS) -c $< -o $@

obj/style/tests/tests.o : src/style/tests/tests.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm obj/* obj/style/* obj/style/tests/* bin/*


