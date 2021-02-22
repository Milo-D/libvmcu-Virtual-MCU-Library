CC		 := -gcc
CFLAGS	 := -O1 -std=gnu11
LDLIBS	 := -lm
MCU_ARCH := -D ATMEGA328P
BUILD    := ./build
OBJ_DIR  := $(BUILD)/objects
APP_DIR  := $(BUILD)/apps
TARGET   := libvmcu.a
INCLUDE  := -I.
SRC      :=                                                 \
	$(wildcard engine/src/analyzer/*.c)                     \
	$(wildcard engine/src/analyzer/modules/sfr/*.c)         \
	$(wildcard engine/src/analyzer/modules/labels/*.c)      \
	$(wildcard engine/src/analyzer/report/*.c)              \
	$(wildcard engine/src/arch/*.c)                         \
	$(wildcard engine/src/collections/*.c)                  \
	$(wildcard engine/src/decoder/*.c)                      \
	$(wildcard engine/src/decomposer/*.c)                   \
   	$(wildcard engine/src/disassembler/*.c)                 \
   	$(wildcard engine/src/instructions/*.c)                 \
	$(wildcard engine/src/system/*.c)                       \
   	$(wildcard engine/src/system/core/*.c)                  \
   	$(wildcard engine/src/system/peripherals/*.c)           \
   	$(wildcard engine/src/misc/*.c)                         \
	$(wildcard engine/src/system/util/*.c)                  \

OBJECTS := $(SRC:%.c=$(OBJ_DIR)/%.o)

all: build $(APP_DIR)/$(TARGET)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) $(MCU_ARCH) $(INCLUDE) -o $@ -c $<


$(OBJ_DIR)/libvmcu.o: $(OBJECTS)
	@mkdir -p $(@D)
	ld -relocatable -o $(OBJ_DIR)/libvmcu.o $(OBJECTS)

$(APP_DIR)/$(TARGET): $(OBJ_DIR)/libvmcu.o
	@mkdir -p $(@D)
	ar rcs $(APP_DIR)/$(TARGET) $(OBJ_DIR)/libvmcu.o

shared: CFLAGS += -fPIC
shared: $(OBJ_DIR)/libvmcu.o
	ld -shared -fPIC -o $(OBJ_DIR)/libvmcu.so $(OBJECTS)

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