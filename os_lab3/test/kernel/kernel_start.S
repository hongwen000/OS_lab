%include "kernel_lib/pm.inc"

align 4

[bits 32]
[section .text]

[extern kernel_pm_setup]
[global kernel_start]
kernel_start:
    xor eax, eax
    mov ax, SEL_KERN_DATA
    mov ds, ax
    mov ax, SEL_KERN_DATA
    mov es, ax
    mov ax, SEL_KERN_VIDEO
    mov gs, ax
    mov ax, SEL_KERN_DATA
    mov ss, ax
    mov esp, 0x7c00         ; memory under 0x7c00 is meaningless in PM, so can be used as stack


; mov the kernel to 0x100000
[extern kernstart]
[extern kernend]
    mov eax, kernend        ; defined in linker script
    mov ecx, kernstart
    sub eax, ecx
    mov ecx, eax
    mov esi, 0x8000
    mov edi, 0x100000
    cld
    rep movsb
    jmp dword SEL_KERN_CODE:relocation_ok

relocation_ok:
    mov	edi, (160*3)+0   ; 160*50 line 3 column 1 
    mov	ah, 00001100b    ; red  
    mov esi, msg_pm  
    call print32

    [extern gdt_init]
    [extern idt_init]
    call gdt_init
    call idt_init
    mov	edi, (160*4)+0   ; 160*50 line 3 column 1 
    mov	ah, 00001100b    ; red  
    mov esi, msg_init_ok
    call print32
    [extern isr_init]
    call isr_init
    [extern kernel_main]
    call kernel_main
    jmp $
;    cli
;    hlt


print32:
    add edi, 160
    push edi
    cld

loop:
    lodsb
    cmp al, 0
    je outloop
    mov	[gs:edi], ax
    add edi, 2
    jmp loop

outloop:
    pop edi
    ret

msg_pm:
    db "In protect mode now.", 0

msg_init_ok:
    db "GDT and IDT init OK", 0



;----------------------------------------------
;IRQ ISR
;----------------------------------------------
int_08_what_to_play dw 0
[global sys_timer_irq_arrive]
sys_timer_irq_arrive dd 0

[global interrupt_timer]
[extern round_robin]
interrupt_timer:
    cli
    __ISR_BEGIN_SAFE_ASM__
;    mov eax, esp
;    push eax
;    call schedule
;    pop eax
;    mov al, 1
;    xchg al, byte[sys_timer_irq_arrive]
;    mov ah, 0x9
;%define INTEVAL 16
;
;    test_up:
;    cmp word[int_08_what_to_play], INTEVAL
;    jge test_left
;    mov al, '|'
;    inc word[int_08_what_to_play]
;    jmp test_input_over
;
;    test_left:
;    cmp word[int_08_what_to_play], INTEVAL * 2
;    jge test_right
;    mov al, '/'
;    inc word[int_08_what_to_play]
;    jmp test_input_over
;
;    test_right:
;    cmp word[int_08_what_to_play], INTEVAL * 3
;    jge test_down
;    mov al, '-'
;    inc word[int_08_what_to_play]
;    jmp test_input_over
;
;    test_down:
;    cmp word[int_08_what_to_play], INTEVAL * 4
;    jge reset_char
;    mov al, '\'
;    inc word[int_08_what_to_play]
;    jmp test_input_over
;
;    reset_char:
;    mov word[int_08_what_to_play], 0
;
;    test_input_over:
;
;    mov word [gs:82],ax

	mov al,0x20
	out 0x20,al
	out 0xA0,al
    __ISR_LEAVE_SAFE_ASM__
    sti
	iret

[global interrupt_kb]
interrupt_kb:
    cli
    __ISR_BEGIN_SAFE_ASM__

        mov ah, 0x9
        mov al, 'O'
        mov word [gs:60],ax
        mov al, 'U'
        mov word [gs:62],ax
        mov al, 'C'
        mov word [gs:64],ax
        mov al, 'H'
        mov word [gs:66],ax
        mov al, 'S'
        mov word [gs:68],ax
        mov al, '!'
        mov word [gs:70],ax
        mov al, 'O'
        mov word [gs:72],ax
        mov al, 'U'
        mov word [gs:74],ax
        mov al, 'C'
        mov word [gs:76],ax
        mov al, 'H'
        mov word [gs:78],ax
        mov al, '!'
        mov word [gs:80],ax

    [extern kb_handler]
    call kb_handler
	mov al,0x20
	out 0x20,al
	out 0xA0,al
    __ISR_LEAVE_SAFE_ASM__
	sti
	iret

[global interrupt_ide]
[extern sys_ide_handler]
interrupt_ide:
    cli
    __ISR_BEGIN_SAFE_ASM__
    call sys_ide_handler
	mov al,0x20
	out 0x20,al
	out 0xA0,al
	__ISR_LEAVE_SAFE_ASM__
	sti
	iret

[global interrupt_90h]
interrupt_90h:
      mov ah, 4ch
[global interrupt_91h]
interrupt_91h:
      cmp ah, 4ch
      jnz panic_91h_func_not_impl
      ;因为引发中断压入的寄存器不会被iret返回，所以要手动清栈
      add sp, 12
      push dword [PSP_USER_RETURN_SEG]
      push dword [PSP_USER_RETURN_ADDR]
      retf
panic_91h_func_not_impl:
      jmp $

[global interrupt_97h]
[extern interrupt_97h_c]
interrupt_97h:
    cli
    __ISR_BEGIN_SAFE_ASM__
    call interrupt_97h_c
    __ISR_LEAVE_SAFE_ASM__
    sti
    iret

[global interrupt_99h]
[extern save]
interrupt_99h:
    cli
    __ISR_BEGIN_SAFE_ASM__
    mov eax, esp
    push eax
    call save
    pop eax
    __ISR_LEAVE_SAFE_ASM__
    sti
    iret



[global interrupt_system_call]
[extern sys_getchar]
[extern sys_current_tty_putchar]
[extern sys_get_timestamp]
interrupt_system_call:
      cli
      __ISR_BEGIN_SAFE_ASM__
func_00:
      cmp ah, 0h
      jnz func_01
      call sys_getchar
      mov dword[esp + __AX__STUB__], eax
      jmp int_98h_end
func_01:
      cmp ah, 1h
      jnz func_02
      push eax
      call sys_current_tty_putchar
      add sp, 4
      jmp int_98h_end
func_02:
      cmp ah, 2h
      jnz func_03
      sti
      sleep_loop:
          cmp byte[sys_timer_irq_arrive], 0
          jz sleep_loop
          mov ax, 0
          xchg al, byte[sys_timer_irq_arrive]
          loop sleep_loop
      jmp int_98h_end
func_03:
    cmp ah, 3h
    jnz func_04
    call sys_get_timestamp
    mov dword[esp + __AX__STUB__], eax
    jmp int_98h_end
func_04:

int_98h_end:
    __ISR_LEAVE_SAFE_ASM__
    sti
    iret

[global set_pit_freq]
set_pit_freq:
    mov al,34h
    out 43h,al
    mov ax,1193182/20
    out 40h,al
    mov al,ah
    out 40h,al
    ret

[global sys_new_proc]
[global sys_sleep]
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
[extern used_pcb_num]
[extern load_ready]

sys_proc_schd:
        cli
        cmp dword[fs:used_pcb_num], 0
        jnz .test_ready
        push ax
                mov al,0x20
                out 0x20,al
                out 0xA0,al
        pop ax
        sti
        iret

        .test_ready:
        cmp dword[fs:load_ready], 0
        jnz .schd
        push ax
                mov al,0x20
                out 0x20,al
                out 0xA0,al
        pop ax
        sti
        iret
        .schd:
        push eax
        mov eax, [fs:pcb_que]
        mov eax, dword[fs:eax + p_status]
        cmp eax, PROC_STAT_READY
        pop eax
        jz .skip_save
    .sys_proc_save:
        pusha
        push ds
        push es
        push fs
        push gs

        push ax
        mov ax, SEL_KERN_DATA
        mov ds, ax
        pop ax

        ;保存
        push esp
        call save
        pop esp
        ;保存完后就可以加载内核段寄存器值了
        .after_save:


        push ax
        mov ax, SEL_KERN_DATA
        mov ds, ax
        mov es, ax
        mov fs, ax
        mov ax, SEL_KERN_VIDEO
        mov gs, ax
        pop ax
        ;交换保存到的值的ds和fs
;        push eax
;        push ebx
;        mov eax, [pcb_que]
;        mov ebx, dword[eax + p_ds]
;        xchg dword[eax + p_fs], ebx
;        mov dword[eax + p_ds], ebx
;        pop ebx
;        pop eax

        call round_robin
    .skip_save:
    [extern sys_proc_set_running]
        call sys_proc_set_running
        mov al,0x20
        out 0x20,al
        out 0xA0,al
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
.proc_ready_switch:
    .test0:
    cmp dword[fs:pid], 0
    jnz .test1
        push dword [fs:pcb0 + p_eflags]
        push dword [fs:pcb0 + p_cs]
        push dword [fs:pcb0 + p_eip]
        jmp .end
    .test1:
    cmp dword[fs:pid], 1
    jnz .test2
        push dword [fs:pcb1 + p_eflags]
        push dword [fs:pcb1 + p_cs]
        push dword [fs:pcb1 + p_eip]
        jmp .end
    .test2:
    cmp dword[fs:pid], 2
    jnz .test3
        push dword [fs:pcb2 + p_eflags]
        push dword [fs:pcb2 + p_cs]
        push dword [fs:pcb2 + p_eip]
        jmp .end
    .test3:
        push dword [fs:pcb3 + p_eflags]
        push dword [fs:pcb3 + p_cs]
        push dword [fs:pcb3 + p_eip]
        jmp .end
    .end:
	sti
        iret


sys_new_proc:
    push edx
    push ecx
    mov edx, [pcb_que]
    mov ecx, dword[edx + p_id]
    .chkpid:
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
.chkpnt:
    push esp
    call save
    pop esp
.edit_esp:
    mov edx, [pcb_que]
    mov ecx, dword[edx + p_id]
    inc ecx
    shl ecx, 0xc
    mov dword[edx + p_esp], ecx
    mov dword[edx + p_eip], .proc_ready_begin
    add esp, 0x30
    iret

.proc_ready_begin:
    mov ds, ax
    mov es, ax
    mov word[USER_LOAD_ADDR - LEN_PSP], OPCODE_INT
    mov word[USER_LOAD_ADDR - LEN_PSP + 2], 0x90
    mov dword[PSP_USER_RETURN_ADDR], .return_point
    mov word[PSP_USER_RETURN_SEG], cs
    push ebx
    push dword USER_LOAD_ADDR
.before_jump:
    retf
.return_point:
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
    .sleep_loop:
        cmp byte[sys_timer_irq_arrive], 0
        jz .sleep_loop
        mov ax, 0
        xchg al, byte[sys_timer_irq_arrive]
        loop .sleep_loop
    popa
    pop bp
    ret