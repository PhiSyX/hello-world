section .text
global _start

_start:
    mov rdi, 1   ; 1 = false
                 ; 0 = true
    mov rax, 60
    syscall

;
; sh
;
; input:
; ./exit_code.out 
; echo $?
;
; output:
; > 1
;
