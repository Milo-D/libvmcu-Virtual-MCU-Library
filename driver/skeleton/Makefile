BUILD    := ../../build
APP_DIR  := $(BUILD)/apps
OBJ_DIR  := $(BUILD)/objects
TARGET   := skeleton
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

driver: skeleton.c
	@cd ..;cd ..;make clean all
	@mkdir -p $(OBJ_DIR)/driver/$(TARGET)
	gcc $(INCLUDE) -c skeleton.c -o $(OBJ_DIR)/driver/$(TARGET).o
	gcc -o $(APP_DIR)/$(TARGET) $(OBJ_DIR)/driver/$(TARGET).o -L$(APP_DIR)/ $(LIBS)