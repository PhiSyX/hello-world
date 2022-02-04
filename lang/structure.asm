; Les commentaires commencent par un point-virgule (;).

label:  ; Pour se débarasser des adresses numériques, on utilisera des labels.
        ; Les labels sont des noms lisibles pour les humains et qui sont 
        ; associés à des adresses numériques.

global _start ; correspond au label `_start:`, il doit être déclaré globalement

; Généralement divisé en sections et chaque section à son utilité.
; Il s'agit d'une directive.

section .data ; pour les variables globales, données disponibles à chaque 
              ; instant de l'exécution du programme.
test1: db 5, 16, 8, 4, 2, 1 ; La directive db est utilisé pour créer 
                            ; des données en bytes.
test2: dw 999                ; dw égal à 2 bytes 
test3: dd 0x00, 0x00         ; dd égal à 4 bytes 
test4: dq 0x1122334455667788 ; dq égal à 8 bytes
test5: times 999 db 42       ; times est une directive qui permet de répéter 
                             ; x fois une commande.

section .text ; contient des instructions

_start: ; nom de la fonction d'entrée du programme.
        ; Il s'agit de la première instruction à exécuter.
