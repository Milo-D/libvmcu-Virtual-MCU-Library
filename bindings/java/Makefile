#https://nachtimwald.com/2017/06/06/wrapping-a-c-library-in-java/

#https://stackoverflow.com/questions/11783632/how-do-i-load-and-use-native-library-in-java

#https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html

#-Djava.library.path must be an absolute path. idk why.
#java -Djava.library.path="/home/alex/libvmcu-Virtual-MCU-Library/bindings/" Main 

#TODO: make it unspecific to the java version
INCLUDE3 := /usr/lib/jvm/java-14-openjdk-amd64/include
INCLUDE4 := /usr/lib/jvm/java-14-openjdk-amd64/include/linux

CFLAGS := -g -fpic -shared

DOINCLUDE := -I$(INCLUDE3) -I$(INCLUDE4)

LIB_VMCU_PATH := ../../build/objects

ALL_JAVA := VMCUSystem.java VMCUReport.java

MODULE_SYSTEM := src/VMCUSystem.h src/VMCUSystem.c src/VMCUSystem.java
MODULE_REPORT := src/VMCUReport.h src/VMCUReport.c src/VMCUReport.java

H_INCLUDE := -I../../engine/include/libvmcu

all: wrapper tests
	cd src; java -Djava.library.path="/home/alex/libvmcu-Virtual-MCU-Library/bindings/java" Tests
	
tests: wrapper
	cd src; javac *.java

wrapper: $(MODULE_SYSTEM) $(MODULE_REPORT) 
	#create shared lib from the jni wrapper
	#and libvmcu.so
	gcc -g -fpic -c src/VMCUSystem.c $(DOINCLUDE) $(H_INCLUDE)
	gcc -g -fpic -c src/VMCUReport.c $(DOINCLUDE) $(H_INCLUDE)
	cp ../../build/objects/libvmcu.so .
	cp libvmcu.so /usr/lib/
	ld -shared -o libvmcu_wrapped.so VMCUSystem.o VMCUReport.o  libvmcu.so -lm -lc
	

clean:
	rm -f *.class *.so *.o src/VMCUSystem.h src/VMCUReport.h src/*.class

#to generate the headers for the c bindings
src/VMCUSystem.h: src/VMCUSystem.java
	cd src; javac -h . VMCUSystem.java
	
src/VMCUReport.h: src/VMCUReport.java
	cd src; javac -h . VMCUReport.java
	
