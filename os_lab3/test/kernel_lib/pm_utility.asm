%include "kernel_lib/pm.inc"
; function used in kernel 

; ********** gdt.c
[global gdt_flush]
[extern gp]

gdt_flush:      
    lgdt [gp]
    mov ax, SEL_KERN_DATA
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov ss, ax
    mov ax, SEL_KERN_VIDEO
    mov gs, ax
    jmp SEL_KERN_CODE:flush2
flush2:
    ret

; ********** idt.c
[global idt_load]
[extern idtp]

idt_load:
    lidt [idtp]
    ret

; ***** exception int 0 - int 31
%macro m_fault 1
[global default_fault%1]
default_fault%1:
    cli
    ; see the int_frame struct
    ; int 8,10-14,17,30 have error code,
    ; int fact int 9 exception happen
    ; on outdataed techhnology, don't care it
    %if %1 != 17 && %1 != 30 && (%1 < 8 || %1 > 14)
        push 0 ; fake error code
    %endif
    push %1    ; int no.
    jmp fault_irq_default_isr
%endmacro

%assign i 0
%rep 32
    m_fault i
    %assign i i + 1
%endrep

; ***** Interrupt Request int 32 - 47
%macro m_irq 1
[global default_irq%1]
default_irq%1:
    cli
    ; see the int_frame struct
    push 0      ;fake error code
    push %1+32  ;int no.
    jmp fault_irq_default_isr
%endmacro

%assign i 0
%rep 16
    m_irq i
%assign i i+1
%endrep

[extern blue_screen]
fault_irq_default_isr:
    pusha
    push ds
    push es
    push fs
    push gs
    mov eax, esp
    push eax
    call blue_screen
    hlt
