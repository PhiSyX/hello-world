section .data
message: db 'Hello, World', 10

section .text
global _start 

_start:
    mov rax, 1       ; le syscall doit être enregistré dans le rax
    mov rdi, 1       ; où écrire? (descripteur)
    mov rsi, message ; où commence la chaîne de caractère?
    mov rdx, 14      ; combien de bytes à écrire
    syscall          ; cette instruction invoque un appel système
    
    mov rax, 60      ; exit = 60
    xor rdi, rdi     ; effece le registre rdi
    syscall