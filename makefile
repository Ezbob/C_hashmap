
CC = gcc
CFLAGS = -std=c99 -Wall
DEBUGFLAGS = -g

MODULES = test hashmap

BUILD_PREFIX = build/
SOURCE_PREFIX = src/
EXEC = hello

SRC_DIR = $(addprefix $(SOURCE_PREFIX),$(MODULES))
BUILD_DIR = $(addprefix $(BUILD_PREFIX), $(MODULES))

VPATH = $(SRC_DIR)

SRC = $(foreach sdir, $(SRC_DIR), $(wildcard $(sdir)/*.c) )
OBJ = $(patsubst $(SOURCE_PREFIX)%.c, $(BUILD_PREFIX)%.o, $(SRC))

EXEC_PATH = $(BUILD_PREFIX)$(EXEC)

# Stuff in define gets evaled twice so we need double $s
define make-c-goal 
$1/%.o: %.c
	$(CC) $(CFLAGS) -c $$< -o $$@
endef

.PHONY: all checkdir clean

all: checkdirs $(EXEC_PATH)

$(EXEC_PATH): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

# creates the build file subtree
checkdirs: $(BUILD_DIR)

$(BUILD_DIR): 
	mkdir -p $@

clean:
	$(RM) -rf $(BUILD_PREFIX)

# the following line creates all the implicit rules for the subdirectories
$(foreach bdir, $(BUILD_DIR), $(eval $(call make-c-goal, $(bdir))))