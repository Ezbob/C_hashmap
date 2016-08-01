########
#
# Makefile that finds C-source files and generates compilation rules for
# use with the GCC compiler.
#
# This makefile relies a specific source tree organization created by the user:
#	- SOURCE_PREFIX: this director contains the different MODULES directories which in turn 
# contains the .c source files for the project
#	- TEST_PREFIX: contains the TEST_MODULES directories which contains the .c source files for 
# the project's tests
#	- INCLUDES_PREFIX: contains headers for the projects 
# 
# Using this source tree organization the following phony targets are availiable:
# 	- all: Default target. Compiles code in SOURCE_PREFIX and creates the EXEC exectutable in
# the BUILD_PREFIX folder under the "bin/" folder
#	- tests: Compiles the test code and creates the TEST_EXEC executable in
# the BUILD_PREFIX folder under the "bin/" folder
#	- run_tests: Relies on the "tests" target and then executes the TEST_EXEC
#	- clean: removes the BUILD_PREFIX
#	- testcheckdirs: Creates the directory tree for the compilation output from compiling the tests
#	- checkdirs: Creates the directory tree for the compilation output from compiling the EXEC executable
#
########
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic -Wshadow -Wpointer-arith -Wcast-qual \
        -Wstrict-prototypes -Wmissing-prototypes

# Modules within source directory and test directory
MODULES = hashmap
TEST_MODULES = unit

# Directory names
BUILD_PREFIX = build/
SOURCE_PREFIX = src/
TEST_PREFIX = tests/
INCLUDE_PREFIX = include/

BIN_PATH = $(addprefix $(BUILD_PREFIX), bin/)

# "default" package
EXEC = map
EXEC_PATH = $(addprefix $(BIN_PATH), $(EXEC))
SRC_DIR = $(addprefix $(SOURCE_PREFIX), $(MODULES))
BUILD_DIR = $(addprefix $(BUILD_PREFIX), $(MODULES))
# Finds all sources files in the source tree (absolute paths)
SRC = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.c)) 
# Simple substitution for all object files (absolute paths)
OBJ = $(patsubst $(SOURCE_PREFIX)%.c, $(BUILD_PREFIX)%.o, $(SRC))

# test package
TEST_EXEC = test_map
TEXEC_PATH = $(addprefix $(BIN_PATH), $(TEST_EXEC))
TEST_SRC_DIR = $(addprefix $(TEST_PREFIX), $(TEST_MODULES))
TEST_DIR = $(addprefix $(BUILD_PREFIX), $(TEST_MODULES))
T_SRC = $(foreach tdir, $(TEST_SRC_DIR), $(wildcard $(tdir)/*.c))
T_OBJ = $(patsubst $(TEST_PREFIX)%.c, $(BUILD_PREFIX)%.o, $(T_SRC))

# Make now knows our source folders
VPATH = $(SRC_DIR) $(TEST_SRC_DIR) 

.PHONY: all test checkdirs testcheckdirs clean run_tests

all: checkdirs $(EXEC_PATH)

tests: testcheckdirs $(TEXEC_PATH)

run_tests: tests
	exec $(TEXEC_PATH)

$(EXEC_PATH): $(OBJ)
	$(CC) -iquote$(INCLUDE_PREFIX) $(CFLAGS) $^ -o $@

$(TEXEC_PATH): $(OBJ) $(T_OBJ)
	$(CC) -iquote$(INCLUDE_PREFIX) $(CFLAGS) $^ -o $@

# creates the build file subtree
testcheckdirs: checkdirs $(TEST_DIR)

checkdirs: $(BUILD_DIR) $(BIN_PATH)
	
clean:
	rm -rf $(BUILD_PREFIX)

## Auto-generating implicit rules

# Stuff in define gets evaled twice so we need double $s

GEN_DIRS = $(BIN_PATH) $(BUILD_DIR) $(TEST_DIR)
GEN_C_RULES = $(BUILD_DIR) $(TEST_DIR)

define make-c-goal 
$1/%.o: %.c
	$(CC) -iquote$(INCLUDE_PREFIX) $(CFLAGS) -c $$< -o $$@
endef

define make-dirs
$1:
	mkdir -p $$@
endef

# the following line creates all the implicit rules for the subdirectories
$(foreach bdir, $(GEN_C_RULES), $(eval $(call make-c-goal, $(bdir))))
$(foreach dir, $(GEN_DIRS), $(eval $(call make-dirs, $(dir))))
