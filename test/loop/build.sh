nasm -f elf hello.asm                         # this will produce hello.o ELF object file
ld -m elf_i386 -s -o hello hello.o                        # this will produce hello executable

