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
[extern schedule]
interrupt_timer:
    cli
    __ISR_BEGIN_SAFE_ASM__
;    mov eax, esp
;    push eax
;    call schedule
;    pop eax
    mov al, 1
    xchg al, byte[sys_timer_irq_arrive]
    mov ah, 0x9
%define INTEVAL 16

    test_up:
    cmp word[int_08_what_to_play], INTEVAL
    jge test_left
    mov al, '|'
    inc word[int_08_what_to_play]
    jmp test_input_over

    test_left:
    cmp word[int_08_what_to_play], INTEVAL * 2
    jge test_right
    mov al, '/'
    inc word[int_08_what_to_play]
    jmp test_input_over

    test_right:
    cmp word[int_08_what_to_play], INTEVAL * 3
    jge test_down
    mov al, '-'
    inc word[int_08_what_to_play]
    jmp test_input_over

    test_down:
    cmp word[int_08_what_to_play], INTEVAL * 4
    jge reset_char
    mov al, '\'
    inc word[int_08_what_to_play]
    jmp test_input_over

    reset_char:
    mov word[int_08_what_to_play], 0

    test_input_over:

    mov word [gs:82],ax

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
interrupt_99h:
    cli
    __ISR_BEGIN_SAFE_ASM__
    mov eax, esp
    push eax
    call schedule
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