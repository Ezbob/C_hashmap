CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -Wpedantic

# Modules within source directory and test directory
MODULES = hashmap
TEST_MODULES = unit

# Directory names
BUILD_PREFIX = build/
SOURCE_PREFIX = src/
TEST_PREFIX = tests/

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
	$(CC) $(CFLAGS) $^ -o $@

$(TEXEC_PATH): $(OBJ) $(T_OBJ)
	$(CC) $(CFLAGS) $^ -o $@

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
	$(CC) $(CFLAGS) -c $$< -o $$@
endef

define make-dirs
$1:
	mkdir -p $$@
endef

# the following line creates all the implicit rules for the subdirectories
$(foreach bdir, $(GEN_C_RULES), $(eval $(call make-c-goal, $(bdir))))
$(foreach dir, $(GEN_DIRS), $(eval $(call make-dirs, $(dir))))
