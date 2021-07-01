TARGET   := integration
LIB_DIR  := ../../build-release
LIB_DBG	 := ../../build-debug
INCLUDE  := -I../../engine/include/libvmcu/
LIBS	 := -lvmcu -lm

all: integration_test.c decoder/decoder_test.o system/system_test.o
	gcc $(INCLUDE) -c integration_test.c -o $(TARGET).o
	gcc -o $(TARGET) $(TARGET).o decoder/decoder_test.o system/system_test.o -L$(LIB_DIR)/ $(LIBS)

	@-rm $(TARGET).o
	@-rm decoder/decoder_test.o
	@-rm system/system_test.o

debug: integration_test.c decoder/decoder_test.o system/system_test.o
	gcc $(INCLUDE) -c integration_test.c -o $(TARGET).o
	gcc -o $(TARGET) $(TARGET).o decoder/decoder_test.o system/system_test.o -L$(LIB_DBG)/ $(LIBS)

	@-rm $(TARGET).o
	@-rm decoder/decoder_test.o
	@-rm system/system_test.o

decoder/decoder_test.o: decoder/decoder_test.c decoder/decoder_test.h
	gcc $(INCLUDE) -c decoder/decoder_test.c -o decoder/decoder_test.o

system/system_test.o: system/system_test.c system/system_test.h
	gcc $(INCLUDE) -c system/system_test.c -o system/system_test.o

clean:
	@-rm $(TARGET)
	@-rm $(TARGET).o
	@-rm decoder/decoder_test.o
	@-rm system/system_test.o

