/* Implementation of Kernel Syscalls */

// C Headers
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// C++ Headers
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>

// Project Headers
#include "syscall.hpp"
#include "ehandling.hpp"
#include "menus.hpp"
#include "simsys.hpp"

#define STDIN 0
#define STDOUT 1
#define STDERR 2

#define PR_DELAY 2

using namespace std;

void call_kernel(SimSys *sys) {

	int routine = sys->read_reg("eax");

	switch(routine) {

		case 1: sys_exit(sys->read_reg("ebx")); 
				sys->exit();

				break;
				
		case 2: /* sys_fork(...) */

				break;
				
		case 3: sys_read(sys);
		
				break;

		case 4: sys_write(sys);

				break;

		default: print_delay("eax stores invalid SysCall.", PR_DELAY, false); break;
 	}
}

void sys_exit(int exit_code) {

	string select;
	string exit_msg = "Program Exit. Code: " + to_string(exit_code);

	do {

		prog_int_menu(exit_msg);

		cout << ">>> ";
		getline(cin, select);
		
	} while(select != "c");
}

void sys_read(SimSys *sys) {

	/* Work in Progress */

	string select, buffer;
	string int_msg = "Reading from ";
	
	int fd = sys->read_reg("ebx");

	if(fd == STDIN) {
	
		buffer = prog_stdin_menu();

		/* Now store this data */

		return;
	}

	int data_size = sys->read_reg("edx");
	ssize_t total = read(fd, (void*) buffer.c_str(), data_size);

	if(total < 0)
		buffer = strerror(errno);

	int_msg += to_string(fd);
	int_msg += " ---> ";

	do {

		prog_int_menu(int_msg + buffer);

		cout << ">>> ";
		getline(cin, select);
		
	} while(select != "c");
}

void sys_write(SimSys *sys) {

	string select;
	string std[3] = { "stdin", "stdout", "stderr" };

	int fd = sys->read_reg("ebx");
	string written = to_string(sys->read_reg("ecx"));

	string int_msg = "Writing to ";

	if(fd >= 0 && fd <= 3) {
	
		int_msg += std[fd];
		
	} else {

		int data_size = sys->read_reg("edx");
		ssize_t total = write(fd, (void*) written.c_str(), data_size);

		if(total != data_size)
			written = strerror(errno);

		int_msg += to_string(fd);
	}
	
	int_msg += " --> ";

	do {
	
		prog_int_menu(int_msg + written);

		cout << ">>> ";
		getline(cin, select);
		
	} while(select != "c");
}





