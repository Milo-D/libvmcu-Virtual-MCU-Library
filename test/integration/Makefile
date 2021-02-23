BUILD    := ../../build
APP_DIR  := $(BUILD)/apps
OBJ_DIR  := $(BUILD)/objects
TARGET   := integration
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

DECODER_OBJ_PATH := $(OBJ_DIR)/test/$(TARGET)/decoder

TEST_OBJ := $(OBJ_DIR)/test/$(TARGET)/$(TARGET).o	\
			$(DECODER_OBJ_PATH)/decoder_test.o		\

test: integration_test.c $(DECODER_OBJ_PATH)/decoder_test.o
	@cd ..;cd ..;make all
	@mkdir -p $(OBJ_DIR)/test/$(TARGET)
	gcc $(INCLUDE) -c integration_test.c -o $(OBJ_DIR)/test/$(TARGET)/$(TARGET).o
	@pwd
	gcc -o $(APP_DIR)/$(TARGET) $(TEST_OBJ) -L$(APP_DIR)/ $(LIBS)

$(DECODER_OBJ_PATH)/decoder_test.o: decoder/decoder_test.h decoder/decoder_test.c
	@mkdir -p $(DECODER_OBJ_PATH)
	gcc $(INCLUDE) -c decoder/decoder_test.c -o $(DECODER_OBJ_PATH)/decoder_test.o