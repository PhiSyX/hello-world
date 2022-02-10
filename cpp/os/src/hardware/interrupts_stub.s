.set IRQ_BASE, 0x20

.section .text
.extern _ZN16InterruptManager16handle_interruptEjj

# $ nm obj/interrupt.o

.macro handle_interrupt_exception number
.global _ZN16InterruptManager20handle_exception\number\()Ev
_ZN16InterruptManager20handle_exception\number\()Ev:
    movb $\number, (interrupt_number)
    jmp int_bottom
.endm

.macro handle_interrupt_request number
.global _ZN16InterruptManager28handle_interrupt_request\number\()Ev
_ZN16InterruptManager28handle_interrupt_request\number\()Ev:
    movb $\number + IRQ_BASE, (interrupt_number)
    pushl $0
    jmp int_bottom
.endm

handle_interrupt_exception 0x00
handle_interrupt_exception 0x01
handle_interrupt_exception 0x02
handle_interrupt_exception 0x03
handle_interrupt_exception 0x04
handle_interrupt_exception 0x05
handle_interrupt_exception 0x06
handle_interrupt_exception 0x07
handle_interrupt_exception 0x08
handle_interrupt_exception 0x09
handle_interrupt_exception 0x0A
handle_interrupt_exception 0x0B
handle_interrupt_exception 0x0C
handle_interrupt_exception 0x0D
handle_interrupt_exception 0x0E
handle_interrupt_exception 0x0F
handle_interrupt_exception 0x10
handle_interrupt_exception 0x11
handle_interrupt_exception 0x12
handle_interrupt_exception 0x13

handle_interrupt_request 0x00
handle_interrupt_request 0x01
handle_interrupt_request 0x02
handle_interrupt_request 0x03
handle_interrupt_request 0x04
handle_interrupt_request 0x05
handle_interrupt_request 0x06
handle_interrupt_request 0x07
handle_interrupt_request 0x08
handle_interrupt_request 0x09
handle_interrupt_request 0x0A
handle_interrupt_request 0x0B
handle_interrupt_request 0x0C
handle_interrupt_request 0x0D
handle_interrupt_request 0x0E
handle_interrupt_request 0x0F
handle_interrupt_request 0x31
handle_interrupt_request 0x80

int_bottom:
    # pusha
    # pushl %ds
    # pushl %es
    # pushl %fs
    # pushl %gs

    pushl %ebp
    pushl %edi
    pushl %esi

    pushl %edx
    pushl %ecx
    pushl %ebx
    pushl %eax


    pushl %esp
    push (interrupt_number)
    call _ZN16InterruptManager16handle_interruptEhj
    # add %esp, 6
    mov %eax, %esp

    popl %eax
    popl %ebx
    popl %ecx
    popl %edx

    popl %esi
    popl %edi
    popl %ebp
    # pop %gs
    # pop %fs
    # pop %es
    # pop %ds
    # popa

    add $4, %esp

.global _ZN16InterruptManager16interrupt_ignoreEv
_ZN16InterruptManager16interrupt_ignoreEv:

    iret

.data
    interrupt_number: .byte 0

