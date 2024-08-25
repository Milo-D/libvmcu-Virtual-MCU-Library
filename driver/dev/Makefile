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

# Default target: build with static debug library
all: debug

# Rules for linking with static libraries in debug mode
debug: CFLAGS += -g -O0
debug: LIBS = $(STATIC_LIB_DEBUG)
debug: $(OBJ) $(STATIC_LIB_DEBUG)
	@echo "Building with Debug Static Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(LIBS)

# Rules for linking with shared libraries in debug mode
debug-shared: CFLAGS += -g -O0
debug-shared: LIBS = $(SHARED_LIB_DEBUG)
debug-shared: $(OBJ) $(SHARED_LIB_DEBUG)
	@echo "Building with Debug Shared Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(LIBS)

# Rules for linking with static libraries in release mode
release: CFLAGS += -O2
release: LIBS = $(STATIC_LIB_RELEASE)
release: $(OBJ) $(STATIC_LIB_RELEASE)
	@echo "Building with Release Static Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(LIBS)

# Rules for linking with shared libraries in release mode
release-shared: CFLAGS += -O2
release-shared: LIBS = $(SHARED_LIB_RELEASE)
release-shared: $(OBJ) $(SHARED_LIB_RELEASE)
	@echo "Building with Release Shared Library..."
	$(CC) $(CFLAGS) -o dev $(OBJ) $(LIBS)

# Compile dev.c into dev.o
dev.o: dev.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c dev.c -o dev.o

# Clean up generated files
clean:
	rm -f $(OBJ) dev
