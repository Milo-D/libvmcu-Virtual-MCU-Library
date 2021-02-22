
all: ../../libvmcu_wrapped.so
	cp -f ../../VMCUSystem.java .
	cp -f ../../VMCUReport.java .
	cp -f ../../Utils.java .
	javac VMCUSystem.java VMCUReport.java Utils.java BasicExample.java
	rm -f VMCUSystem.java VMCUReport.java Utils.java

clean:
	rm -f *.class *.so
