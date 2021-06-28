BUILD    := ../../build
APP_DIR  := $(BUILD)/apps
OBJ_DIR  := $(BUILD)/objects
TARGET   := rwaccess
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

driver: rwaccess.c
	@cd ..;cd ..;make all
	@mkdir -p $(OBJ_DIR)/driver/$(TARGET)
	gcc $(INCLUDE) -c rwaccess.c -o $(OBJ_DIR)/driver/$(TARGET)/$(TARGET).o
	gcc -o $(APP_DIR)/$(TARGET) $(OBJ_DIR)/driver/$(TARGET)/$(TARGET).o -L$(APP_DIR)/ $(LIBS)