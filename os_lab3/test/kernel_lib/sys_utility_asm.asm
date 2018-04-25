%include "kernel_lib/pm.inc"
[BITS 32]

[global sys_new_proc]
[global sys_sleep]
[extern sys_timer_irq_arrive]
[extern save]
[extern pcb_que]

struc pcb_t
    p_id:     resd    1
    p_next:   resd    1
    p_status: resd    1
    p_gs:     resd    1
    p_fs:     resd 1
    p_es:     resd 1
    p_ds: resd 1
    p_edi: resd 1
    p_esi: resd 1
    p_ebp: resd 1
    p_esp: resd 1
    p_ebx: resd 1
    p_edx: resd 1
    p_ecx: resd 1
    p_eax: resd 1
    p_eip: resd 1
    p_cs: resd 1
    p_eflags: resd 1
endstruc

[global sys_proc_schd]
[extern round_robin]
sys_proc_schd:
    sys_proc_save:
        pusha
        push ds
        push es
        push fs
        push gs
        push esp
        call save
        pop esp
        call round_robin
        jmp schd_schedule_asm


[global schedule_asm]
[extern pcb0]
[extern pcb1]
[extern pcb2]
[extern pcb3]
pid dd 0
schedule_asm:
    add esp, 4
schd_schedule_asm:
    mov eax, [pcb_que]
    mov eax, dword[eax + p_id]
    mov dword[pid], eax
    mov eax, [pcb_que]
    push dword [eax + p_esp]
    push dword [eax + p_eax]
    push dword [eax + p_ecx]
    push dword [eax + p_edx]
    push dword [eax + p_ebx]
    push dword [eax + p_ebp]
    push dword [eax + p_esi]
    push dword [eax + p_edi]
    push dword [eax + p_ds]
    push dword [eax + p_es]
    push dword [eax + p_fs]
    push dword [eax + p_gs]
    pop gs
    pop fs
    pop es
    pop ds
    pop edi
    pop esi
    pop ebp
    pop ebx
    pop edx
    pop ecx
    pop eax
    pop esp
proc_ready_switch:
    .test0
    cmp dword[pid], 0
    jnz .test1
        push dword [fs:pcb0 + p_eflags]
        push dword [fs:pcb0 + p_cs]
        push dword [fs:pcb0 + p_eip]
        iret
    .test1
    cmp dword[pid], 1
    jnz .test2
        push dword [fs:pcb1 + p_eflags]
        push dword [fs:pcb1 + p_cs]
        push dword [fs:pcb1 + p_eip]
        iret
    .test2
    cmp dword[pid], 2
    jnz .test3
        push dword [fs:pcb2 + p_eflags]
        push dword [fs:pcb2 + p_cs]
        push dword [fs:pcb2 + p_eip]
        iret
    .test3
        push dword [fs:pcb3 + p_eflags]
        push dword [fs:pcb3 + p_cs]
        push dword [fs:pcb3 + p_eip]
        iret


sys_new_proc:
    push edx
    push ecx
    mov edx, [pcb_que]
    mov ecx, dword[edx + p_id]
    chkpid:
    mov ebx, ecx
    shl ebx, 4
    add ebx, 0x40
    mov eax, ebx
    add eax, 0x8
    pop ecx
    pop edx

    pusha
    push ds
    push es
    push fs
    push gs
chkpnt:
    push esp
    call save
    pop esp

edit_esp:
    mov edx, [pcb_que]
    mov ecx, dword[edx + p_id]
    inc ecx
    shl ecx, 0xc
    mov dword[edx + p_esp], ecx
    mov dword[edx + p_eip], proc_ready_begin
    add esp, 0x30
    iret

proc_ready_begin:
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
    pop gs
    pop fs
    pop es
    pop ds
    popa
    leave
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
