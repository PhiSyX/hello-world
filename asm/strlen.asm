global _start

section .data
test_str: db "abcdef", 0

section .text

strlen: 
    xor rax, rax ; rax tiendra la longueur de la chaîne. 
                 ; S'il n'est pas mise à zéro avant, sa valeur sera totalement
                 ; aléatoire.

.loop:
    cmp byte [rdi + rax], 0 ; vérifie si le caractère courant est nul.
    je .end                 ; si c'est le cas, on sort de la boucle.
    inc rax                 ; sinon, on passe au symbole suivant, 
                            ; & incrémente le compteur.
    jmp .loop

.end:
    ret ; rax devrait contenir la valeur de retour.

_start:
    mov rdi, test_str
    call strlen
    mov rdi, rax
    mov rax, 60
    syscall
