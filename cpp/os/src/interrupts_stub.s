.set IRQ_BASE, 0x20

.section .text
.extern _ZN16InterruptManager16handle_interruptEjj

.global _ZN16InterruptManager24ignore_interrupt_requestEv


.macro handle_interrupt_exception number
.global _ZN16InterruptManager24handle_interrupt_exception\number\()Ev
_ZN16InterruptManager24handle_interrupt_exception\number\()Ev:
    movb $\number, (interrupt_number)
    jmp int_bottom
.endm

.macro handle_interrupt_request number
.global _ZN16InterruptManager28handle_interrupt_request\number\()Ev
_ZN16InterruptManager28handle_interrupt_request\number\()Ev:
    movb $\number, (interrupt_number)
    jmp int_bottom
.endm

handle_interrupt_request 0x00
handle_interrupt_request 0x01

int_bottom:
    pusha
    pushl %ds
    pushl %es
    pushl %fs
    pushl %gs

    pushl %esp
    push (interrupt_number)
    call _ZN16InterruptManager16handle_interruptEjj
  ; add $5, %esp
    mov %eax, %esp

    popl %gs
    popl %fs
    popl %es
    popl %ds
    popa

_ZN16InterruptManager24ignore_interrupt_requestEv:

    iret

.data
    interrupt_number: .byte 0

