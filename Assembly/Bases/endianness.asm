section .data
test1: dq 0x1122334455667788
test2: db 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88
LF: db 10
codes: db '0123456789abcdef'

section .text
global _start

println: 
    mov rax, 1  ; sys_write
    mov rdi, 1  ; stdout
    mov rsi, LF ; d'où provient la donnée
    mov rdx, 1  ; nombre de bytes à écrire
    syscall
    ret

print_x:
    mov rax, rdi
    mov rdi, 1
    mov rdx, 1
    mov rcx, 64     ; jusqu'où doit-on aller avec rax

iter:
    push rax        ; sauvegarde la valeur initiale de rax
    sub rcx, 4
    sar rax, cl     ; 60, 56, 52, ... 4, 0
    and rax, 0xF    ; Efface tous les bits sauf les quatres plus bas
    lea rsi, [codes + rax] ; prend le code hexa correspondant
    mov rax, 1
    push rcx        ; syscall casse le rcx
    syscall         ; rax = 1 (31)      | write
                    ; rdi = 1 (stdout)  | stdout
                    ; rsi = adresse du caractère
    pop rcx
    pop rax
    test rcx, rcx   ; rcx = 0 lorsque tous les chiffres sont affichés
    jnz iter
    ret

_start:
    mov rdi, [test1]
    call print_x
    call println

    mov rdi, [test2]
    call print_x
    call println

    mov rax, 60
    xor rdi, rdi
    syscall
