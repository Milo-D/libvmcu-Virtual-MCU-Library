#https://nachtimwald.com/2017/06/06/wrapping-a-c-library-in-java/

#https://stackoverflow.com/questions/11783632/how-do-i-load-and-use-native-library-in-java

#https://www3.ntu.edu.sg/home/ehchua/programming/java/JavaNativeInterface.html

#-Djava.library.path must be an absolute path. idk why.
#java -Djava.library.path="/home/alex/libvmcu-Virtual-MCU-Library/bindings/" Main 

#to generate VMCUSystem.h
#javac -h . VMCUSystem.java

#TODO: make it unspecific to the java version
INCLUDE3 := /usr/lib/jvm/java-14-openjdk-amd64/include
INCLUDE4 := /usr/lib/jvm/java-14-openjdk-amd64/include/linux

CFLAGS := -g -fpic -shared

DOINCLUDE := -I$(INCLUDE3) -I$(INCLUDE4)

LIB_VMCU_PATH := ../../build/objects

ALL_JAVA := VMCUSystem.java VMCUReport.java

MODULE_SYSTEM := VMCUSystem.h VMCUSystem.c VMCUSystem.java
MODULE_REPORT := VMCUReport.h VMCUReport.c VMCUReport.java

H_INCLUDE := -I../../engine/include/libvmcu

wrapper: $(MODULE_SYSTEM) $(MODULE_REPORT) VMCUSystem.h VMCUReport.h
	#create shared lib from the jni wrapper (VMCUSystem.c)
	#and libvmcu.so
	gcc -g -fpic -c VMCUSystem.c $(DOINCLUDE) $(H_INCLUDE)
	gcc -g -fpic -c VMCUReport.c $(DOINCLUDE) $(H_INCLUDE)
	cp ../../build/objects/libvmcu.so .
	cp libvmcu.so /usr/lib/
	ld -shared -o libvmcu_wrapped.so VMCUSystem.o VMCUReport.o  libvmcu.so -lm -lc
	

clean:
	rm -f *.class *.so *.o VMCUSystem.h VMCUReport.h

#to generate the headers for the c bindings
VMCUSystem.h: VMCUSystem.java
	javac -h . VMCUSystem.java
	
VMCUReport.h: VMCUReport.java
	javac -h . VMCUReport.java
	
