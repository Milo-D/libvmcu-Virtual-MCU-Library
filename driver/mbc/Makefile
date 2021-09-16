TARGET   := mbc
LIB_DIR  := ../../build-release
LIB_DBG	 := ../../build-debug
OBJ_DIR  := $(BUILD)/objects
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

all: $(TARGET).c
	gcc $(INCLUDE) -c $(TARGET).c -o $(TARGET).o
	gcc -o $(TARGET) $(TARGET).o -L$(LIB_DIR)/ $(LIBS)
	@rm $(TARGET).o

debug: $(TARGET).c
	gcc $(INCLUDE) -c $(TARGET).c -o $(TARGET).o
	gcc -o $(TARGET) $(TARGET).o -L$(LIB_DBG)/ $(LIBS)
	@rm $(TARGET).o

clean:
	@-rm $(TARGET).o
	@-rm $(TARGET)