%include "kernel_lib/pm.inc"
[BITS 32]

[global sys_execve_bin]
sys_execve_bin:
    push ebp
    mov ebp, esp
    pusha         ;这里要保护寄存器!!!
    push ds
    push es

    mov ax, [esp + 0x34]
    mov ebx, [esp + 0x30]
    mov ds, ax
    mov es, ax
    mov word[USER_LOAD_ADDR - LEN_PSP], OPCODE_INT
    mov word[USER_LOAD_ADDR - LEN_PSP + 2], 0x90
    mov dword[PSP_USER_RETURN_ADDR], return_point
    mov word[PSP_USER_RETURN_SEG], cs
    push ebx
    push dword USER_LOAD_ADDR
before_jump:
    retf
return_point:
    mov ax, SEL_KERN_DATA
    pop eax
    mov es, ax
    pop eax
    mov ds, ax
    popa
    leave
    ret
