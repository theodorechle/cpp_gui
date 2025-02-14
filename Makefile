CC=g++
CFLAGS=-std=c++17 -Wall -g -MMD -MP
SDL_CMD=`pkg-config sdl3 sdl3-ttf --cflags --libs`
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
TESTS_DIR=tests
LIB=bin/cpp_gui_lib
TESTS_LIB=cpp_tests/bin/cpp_tests_lib
MAIN=bin/cpp_gui

SRC_MAIN=$(SRC_DIR)/main.cpp

# Subdirectories
SUBDIRS=elements elements/managers elements_style elements_style/managers converters app_utils

# Source files
SRC_MAIN=$(SRC_DIR)/main.cpp
SRC_SUBDIRS=$(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.cpp))
SRC_STYLE=$(wildcard $(SRC_DIR)/style/*.cpp)
SRC_STYLE_TESTS=$(SRC_STYLE) $(wildcard $(TESTS_DIR)/*/*.cpp) $(TESTS_DIR)/main.cpp

# Object files
OBJ_MAIN=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_SUBDIRS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SUBDIRS))
OBJ_STYLE=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE))
OBJ_STYLE_TESTS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE_TESTS))

# Executable targets
BIN_ALL=$(BIN_DIR)/all
BIN_STYLE_TESTS=$(BIN_DIR)/style-tests

# Dependency files
# DEPS=$(OBJ_MAIN:.o=.d) $(OBJ_SUBDIRS:.o=.d) $(OBJ_STYLE:.o=.d) $(OBJ_STYLE_TESTS:.o=.d)

.PHONY: all clean style-tests

# Build the final executable combining exe and style
all: $(MAIN)

lib: $(LIB).a

$(LIB).a: $(OBJ_STYLE) $(OBJ_SUBDIRS)
	@mkdir -p $(BIN_DIR)
	ar -r $@ $^

# Build tests only
style-tests: $(BIN_STYLE_TESTS)

# Build everything except tests (combines exe and style)
$(MAIN): $(SRC_MAIN) $(LIB).a
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_CMD)

# Build the style tests executable (style tests + all style sources)
$(BIN_STYLE_TESTS): $(OBJ_STYLE_TESTS) $(TESTS_LIB).a
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@

$(TESTS_LIB).a:
	$(MAKE) -C cpp_tests -j lib

# Rule for compiling all object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Include dependency files if they exist
# -include $(DEPS)

# Clean all generated files
clean:
	@find obj -mindepth 1 ! -name .gitkeep -delete
	@find bin -mindepth 1 ! -name .gitkeep -delete
