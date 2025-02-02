CC=g++
CFLAGS=-std=c++17 -Wall -g -MMD -MP
SDL_CMD=`pkg-config sdl3 sdl3-ttf --cflags --libs`
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
TESTS_DIR=tests

# Subdirectories
SUBDIRS=elements elements/managers elements_style elements_style/managers converters app_utils

# Source files
SRC_MAIN=$(SRC_DIR)/main.cpp
SRC_SUBDIRS=$(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.cpp))
SRC_STYLE=$(wildcard $(SRC_DIR)/style/*.cpp)
SRC_STYLE_TESTS=$(filter-out $(SRC_DIR)/style/main.cpp, $(SRC_STYLE)) $(TESTS_DIR)/tests.cpp $(wildcard $(TESTS_DIR)/*/*.cpp) $(TESTS_DIR)/main.cpp

# Object files
OBJ_MAIN=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_SUBDIRS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SUBDIRS))
OBJ_STYLE=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE))
OBJ_STYLE_TESTS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE_TESTS))

# Executable targets
BIN_ALL=$(BIN_DIR)/all
BIN_STYLE_TESTS=$(BIN_DIR)/style-tests

# Dependency files
DEPS=$(OBJ_MAIN:.o=.d) $(OBJ_SUBDIRS:.o=.d) $(OBJ_STYLE:.o=.d) $(OBJ_TESTS:.o=.d)

.PHONY: all clean style-tests

# Build the final executable combining exe and style
all: $(BIN_ALL)

# Build tests only
style-tests: $(BIN_STYLE_TESTS)

# Build everything except tests (combines exe and style)
$(BIN_ALL): $(filter-out $(OBJ_DIR)/style/main.o, $(OBJ_STYLE)) $(OBJ_SUBDIRS) $(OBJ_MAIN)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(SDL_CMD)

# Build the main executable (main.cpp + elements + managers)
$(BIN_MAIN): $(OBJ_MAIN) $(OBJ_SUBDIRS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Build the style tests executable (style tests + all style sources except src/style/main.cpp)
$(BIN_STYLE_TESTS): $(OBJ_STYLE_TESTS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

# Rule for compiling all object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files if they exist
-include $(DEPS)

# Clean all generated files
clean:
	find obj -mindepth 1 ! -name .gitkeep -delete
	find bin -mindepth 1 ! -name .gitkeep -delete