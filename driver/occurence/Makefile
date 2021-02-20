BUILD    := ../../build
APP_DIR  := $(BUILD)/apps
OBJ_DIR  := $(BUILD)/objects
TARGET   := occurence
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

driver: occurence.c
	@cd ..;cd ..;make all
	@mkdir -p $(OBJ_DIR)/driver/$(TARGET)
	gcc -Wall -g $(INCLUDE) -c occurence.c -o $(OBJ_DIR)/driver/$(TARGET).o
	gcc -o $(APP_DIR)/$(TARGET) $(OBJ_DIR)/driver/$(TARGET).o -L$(APP_DIR)/ $(LIBS)
