# Compiler and flags
CC = gcc
LIB_DIR = ../../build

# Directories
DEBUG_LIB_DIR = $(LIB_DIR)/debug/lib
RELEASE_LIB_DIR = $(LIB_DIR)/release/lib
INCLUDE_DIR = ../../src/include/lib

# Library names
LIB_NAME = vmcu
STATIC_LIB_DEBUG = $(DEBUG_LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB_DEBUG = $(DEBUG_LIB_DIR)/lib$(LIB_NAME).so
STATIC_LIB_RELEASE = $(RELEASE_LIB_DIR)/lib$(LIB_NAME).a
SHARED_LIB_RELEASE = $(RELEASE_LIB_DIR)/lib$(LIB_NAME).so

# Source and object files
SRC = dev.c
OBJ = dev.o

# Targets
.PHONY: all clean

# Default target: build with selected library type
all: $(OBJ) $(LIB)
	$(CC) $(CFLAGS) -o dev $(OBJ) $(LIB)

# Rules for linking with static or shared libraries in debug mode
debug: CFLAGS += -g -O0
debug: $(OBJ) $(STATIC_LIB_DEBUG) $(SHARED_LIB_DEBUG)
	@echo "Building with Debug Static Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(STATIC_LIB_DEBUG)

debug-shared: CFLAGS += -g -O0
debug-shared: $(OBJ) $(SHARED_LIB_DEBUG)
	@echo "Building with Debug Shared Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(SHARED_LIB_DEBUG)

# Rules for linking with static or shared libraries in release mode
release: CFLAGS += -O2
release: $(OBJ) $(STATIC_LIB_RELEASE) $(SHARED_LIB_RELEASE)
	@echo "Building with Release Static Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(STATIC_LIB_RELEASE)

release-shared: CFLAGS += -O2
release-shared: $(OBJ) $(SHARED_LIB_RELEASE)
	@echo "Building with Release Shared Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(SHARED_LIB_RELEASE)

# Compile dev.c into dev.o
dev.o: dev.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c dev.c -o dev.o

# Clean up generated files
clean:
	rm -f $(OBJ) dev
