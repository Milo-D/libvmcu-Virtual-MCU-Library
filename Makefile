CC		 := -gcc
CFLAGS	 := -O1 -std=gnu11
LDLIBS	 := -lm -lncurses
MCU_ARCH := -D ATMEGA328P
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := mdx
INCLUDE  := -I engine/include -I debugger/include -I shared/include
SRC      :=                                					\
	$(wildcard engine/src/analyzer/*.c)						\
	$(wildcard engine/src/analyzer/modules/sfr/*.c)			\
	$(wildcard engine/src/analyzer/report/*.c)				\
	$(wildcard engine/src/decoder/*.c)						\
	$(wildcard engine/src/decomposer/*.c)					\
   	$(wildcard engine/src/disassembler/*.c)        			\
   	$(wildcard engine/src/instructions/*.c)        			\
	$(wildcard engine/src/system/*.c)              			\
   	$(wildcard engine/src/system/core/*.c)         			\
   	$(wildcard engine/src/system/peripherals/*.c)  			\
	$(wildcard engine/src/system/util/*.c)					\
	$(wildcard debugger/src/cli/main/*.c)					\
   	$(wildcard debugger/src/cli/debug/*.c)           		\
	$(wildcard debugger/src/cli/util/*.c)					\
   	$(wildcard debugger/src/parser/*.c)              		\
   	$(wildcard debugger/src/printer/*.c)             		\
   	$(wildcard debugger/src/state/main/*.c)					\
   	$(wildcard debugger/src/state/debug/*.c)				\
   	$(wildcard debugger/src/table/*.c)               		\
   	$(wildcard shared/src/collections/*.c)         			\
   	$(wildcard shared/src/misc/*.c)                			\

OBJECTS := $(SRC:%.c=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MCU_ARCH) $(INCLUDE) -o $@ -c $<

$(APP_DIR)/$(TARGET): $(OBJECTS)
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MCU_ARCH) $(INCLUDE) -o $(APP_DIR)/$(TARGET) $(OBJECTS) $(LDLIBS)

.PHONY: all build clean release

build:
	@mkdir -p $(APP_DIR)
	@mkdir -p $(OBJ_DIR)

release: CFLAGS += -O2
release: all

clean:
	@find $(OBJ_DIR) ! -name 'README' -type f -exec rm -f {} +
	@find $(APP_DIR) ! -name 'README' -type f -exec rm -f {} +
	-@rmdir $(OBJ_DIR)/src/*/*
