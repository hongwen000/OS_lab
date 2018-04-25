%include "kernel_lib/pm.inc"
[BITS 32]

[global sys_execve_bin]
[global sys_sleep]
[extern sys_timer_irq_arrive]

sys_execve_bin:
    push ebp
    mov ebp, esp
    pusha         ;这里要保护寄存器!!!
    push ds
    push es
    push ss

    mov ax, [esp + 0x38]
    mov ebx, [esp + 0x34]
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov word[USER_LOAD_ADDR - LEN_PSP], OPCODE_INT
    mov word[USER_LOAD_ADDR - LEN_PSP + 2], 0x90
    mov dword[PSP_USER_RETURN_ADDR], return_point
    mov word[PSP_USER_RETURN_SEG], cs
    push ebx
    push dword USER_LOAD_ADDR
before_jump:
    retf
return_point:
    pop eax
    mov ss, ax
    pop eax
    mov es, ax
    pop eax
    mov ds, ax
    popa
    leave
    add esp, 8
    ret

sys_sleep:
    push bp
    mov bp, sp
    pusha
    mov cx, word[bp+10]
    sleep_loop:
        cmp byte[sys_timer_irq_arrive], 0
        jz sleep_loop
        mov ax, 0
        xchg al, byte[sys_timer_irq_arrive]
        loop sleep_loop
    popa
    pop bp
    ret
