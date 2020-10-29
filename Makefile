CXX      := -gcc
CXXFLAGS := -g -D
LDFLAGS  := -L /usr/lib -lm -lncurses
MCU_ARCH := ATMEGA328P
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := mdx
INCLUDE  := -I include/
SRC      :=                                	\
	$(wildcard src/cli/main/*.c)		\
   	$(wildcard src/cli/debug/*.c)           \
   	$(wildcard src/collections/*.c)         \
   	$(wildcard src/disassembler/*.c)        \
   	$(wildcard src/instructions/*.c)        \
   	$(wildcard src/misc/*.c)                \
   	$(wildcard src/parser/*.c)              \
   	$(wildcard src/printer/*.c)             \
   	$(wildcard src/state/main/*.c)		\
   	$(wildcard src/state/debug/*.c)	\
   	$(wildcard src/system/*.c)              \
   	$(wildcard src/system/core/*.c)         \
   	$(wildcard src/system/peripherals/*.c)  \
   	$(wildcard src/table/*.c)               \

OBJECTS := $(SRC:%.c=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(MCU_ARCH) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) $(MCU_ARCH) $(INCLUDE) -o $(APP_DIR)/$(TARGET) $(OBJECTS) $(LDFLAGS)

.PHONY: all build clean release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

release: CXXFLAGS += -O2
release: all

clean:
	find $(OBJ_DIR) ! -name 'README' -type f -exec rm -f {} +
	find $(APP_DIR) ! -name 'README' -type f -exec rm -f {} +
	-@rmdir $(OBJ_DIR)/src/*
	-@rmdir $(OBJ_DIR)/src
