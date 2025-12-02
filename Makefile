CPP_C=g++
CPP_FLAGS=-std=c++17 -Wall -g -MMD -MP
SDL_CMD=`pkg-config sdl3 sdl3-ttf --cflags --libs`
BIN_DIR=bin
OBJ_DIR=obj/lib
OBJ_TEST_DIR=obj/test
SRC_DIR=src
TESTS_DIR=tests
LIB=bin/cpp_gui_lib
STYLE_LIB=cpp_style/bin/cpp_style_lib
TESTS_LIB=cpp_tests/bin/cpp_tests_lib

# Subdirectories
SUBDIRS=elements/abstracts elements/ui elements/ui/render_nodes converters app_utils style_nodes

# Source files
SRC_MAIN=$(SRC_DIR)/main.cpp
SRC_SUBDIRS=$(foreach dir, $(SUBDIRS), $(wildcard $(SRC_DIR)/$(dir)/*.cpp))
SRC_TESTS=$(wildcard $(TESTS_DIR)/*.cpp) $(wildcard $(TESTS_DIR)/*/*.cpp)

# Object files
OBJ_MAIN=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_MAIN))
OBJ_SUBDIRS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_SUBDIRS))
OBJ_TESTS=$(patsubst $(SRC_DIR)/%.cpp, $(OBJ_TEST_DIR)/%.o, $(SRC_TESTS))

# Executable targets
MAIN=$(BIN_DIR)/cpp_gui
BIN_ALL=$(BIN_DIR)/all
TESTS=$(BIN_DIR)/tests

.PHONY: all clean tests

ifeq ($(DEBUG),1)
CPP_FLAGS += -DDEBUG
endif

all: $(MAIN)

lib: $(LIB).a

tests: $(TESTS)

$(LIB).a: $(OBJ_SUBDIRS)
	@mkdir -p $(BIN_DIR)
	ar -r $@ $^

# Build everything except tests
$(MAIN): $(OBJ_MAIN) $(LIB).a $(STYLE_LIB).a
	@mkdir -p $(BIN_DIR)
	$(CPP_C) $(CPP_FLAGS) -o $@ $^ $(SDL_CMD)

# Build the tests executable (tests + lib)
$(TESTS): $(OBJ_TESTS) $(LIB).a $(TESTS_LIB).a $(STYLE_LIB).a
	@mkdir -p $(BIN_DIR)
	$(CPP_C) $(CPP_FLAGS) -o $@ $^

# Rule for compiling all object files
$(OBJ_TEST_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -DDEBUG -c $< -o $@

$(STYLE_LIB).a:
	$(MAKE) -C cpp_style -j lib DEBUG=$(DEBUG)

$(TESTS_LIB).a:
	$(MAKE) -C cpp_tests -j lib DEBUG=$(DEBUG)

# Rule for compiling all object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPP_C) $(CPP_FLAGS) -c $< -o $@

# Clean all generated files
clean:
	@find obj -mindepth 1 ! -name .gitkeep -delete
	@find bin -mindepth 1 ! -name .gitkeep -delete
	$(MAKE) -C cpp_style clean
	$(MAKE) -C cpp_tests clean