section .text                ;must be declared for linker (ld)
global _start                ;                    
_start:                      ;                    
        jmp main             ;                    
main:                        ;                    
        mov eax,3            ;iconst 3            
        push eax             ;iconst 3            
        pop eax              ;dup                 
        push eax             ;dup                 
        push eax             ;dup                 
loopstart32677:              ;                    
        mov eax,0            ;iconst 0            
        push eax             ;iconst 0            
        pop eax              ;                    
        pop ebx              ;                    
        cmp eax,ebx          ;                    
        je eq_push140176     ;                    
        mov eax,0            ;                    
        push eax             ;                    
        jmp eq_end40176      ;                    
eq_push140176:               ;                    
        mov eax,1            ;                    
        push eax             ;                    
        jmp eq_end40176      ;                    
eq_end40176:                 ;                    
        pop eax              ;                    
        mov ebx,1            ;                    
        cmp eax,ebx          ;                    
        je ifgoto_true25879  ;                    
        jmp ifgoto_end25879  ;                    
ifgoto_true25879:            ;                    
        jmp loopend32677     ;                    
ifgoto_end25879:             ;                    
        mov eax,120          ;iconst 120          
        push eax             ;iconst 120          
        mov eax,4            ;sys_writ            
        mov ebx,1            ;std_out             
        mov ecx,esp          ;                    
        mov edx,1            ;value length        
        int 0x80             ;call kernel         
        pop edx              ;                    
        mov edx,0            ;                    
        push edx             ;                    
        mov eax,1            ;iconst 1            
        push eax             ;iconst 1            
        pop ebx              ;sub                 
        pop eax              ;sub                 
        sub eax,ebx          ;sub                 
        push eax             ;sub                 
        pop eax              ;dup                 
        push eax             ;dup                 
        push eax             ;dup                 
        jmp loopstart32677   ;                    
loopend32677:                ;                    
        pop eax		         ;                    
        mov eax,0            ;iconst 0            
        push eax             ;iconst 0            
        mov eax,1            ;exit: sytem call number (sys_exit)
        pop ebx              ;exit: pop exit status code from stack
        int 0x80             ;call kernel         
        pop eax              ;exit                
        pop ebx              ;exit                
        add eax,ebx          ;exit                
        push eax             ;exit                
section .data                ;                    
section .bss                 ;                    
