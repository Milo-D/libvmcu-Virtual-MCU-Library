_start:
jmp main
main:
mov eax, 0
mov ebx, 1
mov ecx, 2
mov edx, 3
jmp second
second:
mov edx, 0
mov ecx, 1
mov ebx, 2
mov eax, 3
jmp main