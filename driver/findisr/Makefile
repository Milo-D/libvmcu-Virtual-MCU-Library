BUILD    := ../../build
APP_DIR  := $(BUILD)/apps
OBJ_DIR  := $(BUILD)/objects
TARGET   := findisr
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

driver: findisr.c
	@cd ..;cd ..;make all
	@mkdir -p $(OBJ_DIR)/driver/$(TARGET)
	gcc $(INCLUDE) -c findisr.c -o $(OBJ_DIR)/driver/$(TARGET).o
	gcc -o $(APP_DIR)/$(TARGET) $(OBJ_DIR)/driver/$(TARGET).o -L$(APP_DIR)/ $(LIBS)
