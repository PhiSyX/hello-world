section .data
codes:
    db '0123456789ABCDEF'

section .text
global _start
_start:
    mov rax, 0x1122334455667788 ; nombre hexadecimal
    mov rdi, 1
    mov rdx, 1
    mov rcx, 64


; Chaque 4 bits doit être émis comme un chiffre hexadécimal.
; Nous utiliserons le décalage '&' (BITWISE AND) pour les isoler.
; Le résultat est le décalage dans le tableau 'codes'
.loop:
    push rax
    sub rcx, 4
    sar rax, cl     ; cl est un registre, la plus petite du rcx
                    ; rax -- eax -- ax -- ah + al
                    ; rcx -- ecx -- cx -- ch + cl
    and rax, 0xf
    lea rsi, [codes + rax]
    mov rax, 1
    push rcx
    syscall         ; sys_write
    pop rcx
    pop rax
    test rcx, rcx   ; if rcx == 1, then 1 else 0
    jnz .loop

    mov rax, 60     ; sys_exit
    xor rdi, rdi
    syscall
