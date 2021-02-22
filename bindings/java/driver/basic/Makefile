
all: ../../libvmcu_wrapped.so
	cp -f ../../src/VMCUSystem.java .
	cp -f ../../src/VMCUReport.java .
	cp -f ../../src/Utils.java .
	javac VMCUSystem.java VMCUReport.java Utils.java BasicExample.java
	rm -f VMCUSystem.java VMCUReport.java Utils.java

clean:
	rm -f *.class *.so
