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

[global sys_context_switch] ;(context_t** p_p_old_context, context_t* p_new_context);
sys_context_switch:
    mov eax, [esp + 4]  ; p_p_old -> eax
    mov edx, [esp + 8]  ; p_new -> edx

    ; eip has been save implicitly when context_switch is called
    ; according to x86 convention, save callee−save registers
    push ebp
    push ebx
    push esi
    push edi

    ; change the value of p_old_context, make it point to new current location of stack
    mov [eax], esp      ; implicitly save esp
    ; kernel stack now point at new location specifiy by p_new_context
    mov esp, edx

    ; restore context from there
    pop edi
    pop esi
    pop ebx
    pop ebp
    ret
