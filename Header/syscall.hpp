/* Kernel SysCall Header */

#ifndef SYSCALL_HPP
#define SYSCALL_HPP

class SimSys;

/* Entry Point */
void call_kernel(SimSys *sys);

/* Called by 'call_kernel' */
void sys_exit(int exit_code);
void sys_read(SimSys *sys);
void sys_write(SimSys *sys);

#endif