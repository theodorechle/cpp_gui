CPP_C=g++
CPP_FLAGS=-std=c++17 -Wall -g -MMD -MP
SDL_CMD=`pkg-config sdl3 sdl3-ttf --cflags --libs`
BIN_DIR=bin
OBJ_DIR=obj/normal
OBJ_TEST_DIR=obj/test
SRC_DIR=src
TESTS_DIR=tests
LIB=bin/cpp_gui_lib
TESTS_LIB=cpp_tests/bin/cpp_tests_lib

# Subdirectories
SUBDIRS=elements/abstracts elements/ui elements/ui/render_nodes style_nodes converters app_utils

# Source files
SRC_MAIN=$(SRC_DIR)/main.cpp
SRC_SUBDIRS=$(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.cpp))
SRC_STYLE=$(wildcard $(SRC_DIR)/style/*.cpp)
SRC_TESTS=$(SRC_STYLE) $(wildcard $(SRC_DIR)/style_nodes/*.cpp) $(wildcard $(TESTS_DIR)/*/*.cpp) $(TESTS_DIR)/main.cpp

# Object files
OBJ_MAIN=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_SUBDIRS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SUBDIRS))
OBJ_STYLE=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_STYLE))
OBJ_TESTS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(SRC_TESTS))

# Executable targets
MAIN=$(BIN_DIR)/cpp_gui
BIN_ALL=$(BIN_DIR)/all
BIN_TESTS=$(BIN_DIR)/tests

# Dependency files
# DEPS=$(OBJ_MAIN:.o=.d) $(OBJ_SUBDIRS:.o=.d) $(OBJ_STYLE:.o=.d) $(OBJ_TESTS:.o=.d)

.PHONY: all clean tests

# $(info origin=$(origin DEBUG))
# $(info value='$(DEBUG)')

ifdef DEBUG
CPP_FLAGS += -DDEBUG
endif

# Build the final executable combining exe and style
all: $(MAIN)

lib: $(LIB).a

tests: $(BIN_TESTS)

$(LIB).a: $(OBJ_STYLE) $(OBJ_SUBDIRS)
	@mkdir -p $(BIN_DIR)
	ar -r $@ $^

# Build everything except tests
$(MAIN): $(OBJ_MAIN) $(LIB).a
	@mkdir -p $(BIN_DIR)
	$(CPP_C) $(CPP_FLAGS) -o $@ $^ $(SDL_CMD)

# Build the tests executable (tests + all style sources)
$(BIN_TESTS): $(OBJ_TESTS) $(TESTS_LIB).a
	@mkdir -p $(BIN_DIR)
	$(CPP_C) $(CPP_FLAGS) -o $@ $^

# Rule for compiling all object files
$(OBJ_TEST_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -DDEBUG -c $< -o $@

$(TESTS_LIB).a:
	$(MAKE) -C cpp_tests -j lib DEBUG=DEBUG

# Rule for compiling all object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -c $< -o $@

# Include dependency files if they exist
# -include $(DEPS)

# Clean all generated files
clean:
	@find obj -mindepth 1 ! -name .gitkeep -delete
	@find bin -mindepth 1 ! -name .gitkeep -delete
	$(MAKE) -C cpp_tests clean