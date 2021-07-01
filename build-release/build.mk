all:
	cmake .. -DCMAKE_BUILD_TYPE=release
	cmake --build . --target all	

clean:
	@-rm -rf CMakeFiles/
	@-rm CMakeCache.txt
	@-rm cmake_install.cmake
	@-rm Makefile
	@-rm libvmcu.a
