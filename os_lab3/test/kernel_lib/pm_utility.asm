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
    mov gs, ax
    mov ss, ax
    jmp SEL_KERN_CODE:flush2
flush2:
    ret

; ********** idt.c
[global idt_load]
[extern idtp]

idt_load:
    lidt [idtp]
    ret

; ********** Interrupt Service Routines


; ; ***** exception int 0 - int 31
; %macro m_fault 1
; [global fault%1]
; fault%1:
;     cli
;     ; int 8,10-14,17,30 have error code, 
;     ; int fact int 9 exception happen 
;     ; on outdataed techhnology, don't care it
;     %if %1 != 17 && %1 != 30 && (%1 < 8 || %1 > 14)
;         push 0 ; fake error code
;     %endif
;     push %1
;     jmp _isr_stub
; %endmacro 

; %assign i 0
; %rep 32
;     m_fault i
;     %assign i i + 1
; %endrep

; ; ***** Interrupt Request int 32 - 47
; %macro m_irq 1
; [global irq%1]
; irq%1:
;     cli
;     push 0
;     push %1+32
;     jmp _isr_stub
; %endmacro

; %assign i 0
; %rep 16 
;     m_irq i   
; %assign i i+1
; %endrep

; ; ***** System Call int 0x80
; [global _syscall]
; _syscall:
;     push 0
;     push 0x80
;     jmp _isr_stub

; ; ***** Unknown Interrupt
; ; 255 is a flag of unknown int
; ; so please note that we can't use it 
; [global isr_unknown]
; isr_unknown:
;     cli
;     push 0
;     push 255
;     jmp _isr_stub

; ; ********** Common Interrupt Process Routine
; ; exists in kern/isr.c 
; [global _isr_stub_ret]
; [extern isr_stub]
; ; a common ISR func, sava the context of CPU 
; ; call C func to process fault
; ; at last restore stack frame
; _isr_stub:
;     pusha
;     push ds
;     push es
;     push fs
;     push gs

;     mov ax, SEL_KERN_DATA
;     mov ds, ax
;     mov es, ax
;     mov fs, ax
;     mov gs, ax
;     mov ss, ax

;     mov eax, esp
;     push eax

;     mov eax,isr_stub
;     call eax
;     pop eax

; _isr_stub_ret:
;     pop gs
;     pop fs
;     pop es
;     pop ds
;     popa
;     add esp, 8
;     iret

; ; ********** context switch: use in proc/proc.c
; ; void context_switch(struct context **old, struct *new);
; [global context_switch]
; context_switch:
;     mov eax, [esp + 4]  ; old
;     mov edx, [esp + 8]  ; new

;     ; eip has been save when call context_switch
;     push ebp
;     push ebx
;     push esi
;     push edi

;     ; switch stack
;     mov [eax], esp      ; save esp
;     mov esp, edx

;     pop edi
;     pop esi
;     pop ebx
;     pop ebp
;     ret

