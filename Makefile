CC=g++
CFLAGS=-std=c++17 -Wall -g -MMD -MP
SDL_CMD=`pkg-config sdl3 --cflags --libs`
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src

# Subdirectories
SUBDIRS=$(SRC_DIR)/elements $(SRC_DIR)/managers $(SRC_DIR)/app_utils

# Source files
SRC_MAIN=$(SRC_DIR)/main.cpp
SRC_SUBDIRS=$(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.cpp))
SRC_STYLE=$(wildcard src/style/*.cpp)
SRC_STYLE_TESTS=$(wildcard src/style/tests/*.cpp)
SRC_TESTS=$(filter-out src/style/main.cpp, $(SRC_STYLE)) $(SRC_STYLE_TESTS)

# Object files
OBJ_MAIN=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_SUBDIRS=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SUBDIRS))
OBJ_STYLE=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE))
OBJ_TESTS=$(patsubst src/%.cpp, $(OBJ_DIR)/%.o, $(SRC_TESTS))

# Executable targets
BIN_MAIN=$(BIN_DIR)/exe
BIN_STYLE=$(BIN_DIR)/style
BIN_ALL=$(BIN_DIR)/all
BIN_STYLE_TESTS=$(BIN_DIR)/style-tests

# Dependency files
DEPS=$(OBJ_MAIN:.o=.d) $(OBJ_SUBDIRS:.o=.d) $(OBJ_STYLE:.o=.d) $(OBJ_TESTS:.o=.d)

.PHONY: all clean style style-tests main

# Build the final executable combining exe and style
all: $(BIN_ALL)

# Build only the main and subdirs
main: $(BIN_MAIN)


# Build style only
style: $(BIN_STYLE)

# Build tests only
style-tests: $(BIN_STYLE_TESTS)

# Build everything except tests (combines exe and style)
$(BIN_ALL): $(OBJ_MAIN) $(OBJ_SUBDIRS) $(filter-out obj/style/main.o, $(OBJ_STYLE))
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(SDL_CMD)

# Build the main executable (main.cpp + elements + managers)
$(BIN_MAIN): $(OBJ_MAIN) $(OBJ_SUBDIRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(SDL_CMD)

# Build the style executable (style sources only)
$(BIN_STYLE): $(OBJ_STYLE)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(SDL_CMD)

# Build the style tests executable (style tests + all style sources except src/style/main.cpp)
$(BIN_STYLE_TESTS): $(OBJ_TESTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(SDL_CMD)

# Rule for compiling all object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@ $(SDL_CMD)

# Include dependency files if they exist
-include $(DEPS)

# Clean all generated files
clean:
	find obj -mindepth 1 ! -name .gitkeep -delete
	find bin -mindepth 1 ! -name .gitkeep -delete