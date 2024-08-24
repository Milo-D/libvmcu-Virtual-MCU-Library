# Compiler and flags
CC = gcc
CFLAGS = -fPIC
DEBUG_FLAGS = -g -O0
RELEASE_FLAGS = -O2
WARNINGS_FLAGS = -Wall -Wextra

# Directories
SRC_DIR = src
INCLUDE_DIR = $(SRC_DIR)/include
BUILD_DIR = build
DEBUG_BUILD_DIR = $(BUILD_DIR)/debug
RELEASE_BUILD_DIR = $(BUILD_DIR)/release

# Library name
LIB_NAME = vmcu

# Output files for debug and release builds
DEBUG_OBJ_DIR = $(DEBUG_BUILD_DIR)/obj
RELEASE_OBJ_DIR = $(RELEASE_BUILD_DIR)/obj
DEBUG_LIB_DIR = $(DEBUG_BUILD_DIR)/lib
RELEASE_LIB_DIR = $(RELEASE_BUILD_DIR)/lib

DEBUG_STATIC_LIB = $(DEBUG_LIB_DIR)/lib$(LIB_NAME).a
DEBUG_SHARED_LIB = $(DEBUG_LIB_DIR)/lib$(LIB_NAME).so
RELEASE_STATIC_LIB = $(RELEASE_LIB_DIR)/lib$(LIB_NAME).a
RELEASE_SHARED_LIB = $(RELEASE_LIB_DIR)/lib$(LIB_NAME).so

# Find all .c files in the source directory
SRCS = $(shell find $(SRC_DIR) -name '*.c')

# Generate corresponding .o files in the debug/release obj directory
DEBUG_OBJS = $(SRCS:$(SRC_DIR)/%.c=$(DEBUG_OBJ_DIR)/%.o)
RELEASE_OBJS = $(SRCS:$(SRC_DIR)/%.c=$(RELEASE_OBJ_DIR)/%.o)

# Check for WARNINGS variable
ifdef WARN
    CFLAGS += $(WARNINGS_FLAGS)
endif

# Targets
.PHONY: all static shared debug release clean clean-debug clean-release

# Default target: Build both debug and release libraries
all: debug release

# prettify makefile :)
vmcu_splash:
	@echo "___  _______   ____  __ __ "
	@echo "\  \/ /     \_/ ___\|  |  \\"
	@echo " \   /  Y Y  \  \___|  |  /"
	@echo "  \_/|__|_|  /\___  >____/ "
	@echo "           \/     \/       "

# Build static and shared libraries for debug
debug: vmcu_splash $(DEBUG_STATIC_LIB) $(DEBUG_SHARED_LIB)

$(DEBUG_STATIC_LIB): $(DEBUG_OBJS)
	@mkdir -p $(DEBUG_LIB_DIR)
	ar rcs $@ $^

$(DEBUG_SHARED_LIB): $(DEBUG_OBJS)
	@mkdir -p $(DEBUG_LIB_DIR)
	$(CC) -shared -o $@ $^

$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Build static and shared libraries for release
release: vmcu_splash $(RELEASE_STATIC_LIB) $(RELEASE_SHARED_LIB)

$(RELEASE_STATIC_LIB): $(RELEASE_OBJS)
	@mkdir -p $(RELEASE_LIB_DIR)
	ar rcs $@ $^

$(RELEASE_SHARED_LIB): $(RELEASE_OBJS)
	@mkdir -p $(RELEASE_LIB_DIR)
	$(CC) -shared -o $@ $^

$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(RELEASE_FLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean up all generated files
clean: clean-debug clean-release

# Clean up only debug build files
clean-debug:
	rm -rf $(DEBUG_BUILD_DIR)

# Clean up only release build files
clean-release:
	rm -rf $(RELEASE_BUILD_DIR)
