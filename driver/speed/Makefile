BUILD    := ../../build
APP_DIR  := $(BUILD)/apps
OBJ_DIR  := $(BUILD)/objects
TARGET   := speed
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

driver: speed.c
	@cd ..;cd ..;make clean all
	@mkdir -p $(OBJ_DIR)/driver/$(TARGET)
	gcc $(INCLUDE) -c speed.c -o $(OBJ_DIR)/driver/$(TARGET)/$(TARGET).o
	gcc -o $(APP_DIR)/$(TARGET) $(OBJ_DIR)/driver/$(TARGET)/$(TARGET).o -L$(APP_DIR)/ $(LIBS)