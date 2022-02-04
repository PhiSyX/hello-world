global _start

section .data
test_str: db "abcdef", 0

section .text

strlen:
                            ; r13 doit être sauvegardé avant l'utilisation et  >  push r13
                            ; restauré avant le retour.
                            ; r13 n'est pas mis à zéro avant utilisation       >  xor, r13, r13

.loop:
    cmp byte [rdi+r13], 0
    je .end
    inc r13
    jmp .loop

.end:
    mov rax, r13
                            ;                                                  >  pop r13
    ret

_start:
    mov rdi, test_str
    call strlen
    mov rdi, rax
    mov rax, 60
    syscall
