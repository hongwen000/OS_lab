;[section .bss]
;align 16
;[section .stack]
;stack_bottom:
;times 16384 db 0
;stack_top:
[BITS 16]
[section .text]
[global kernel_start]
[extern kernel_main]
[extern sys_bios_getchar]
[extern sys_getchar]
[extern kb_handler]
[extern sys_current_tty_putchar]
[extern system_call_getchar]
[extern fun_in_c]
[extern interrupt_33h_c]
[extern interrupt_34h_c]
;FIXED BUG
;要哭了，终于调试出来了
;不能用static的，引用地址时会出问题
;必须要用全局变量
[extern ctrl_shift_status]
[global sys_int08_arrive]

kernel_start:

    mov al,34h
	out 43h,al
	mov ax,1193182/25
	out 40h,al
	mov al,ah
	out 40h,al
    
    mov byte[ctrl_shift_status], 0
    push interrupt_08h
    push 08h * 4
    call install_int
    add sp, 4

    push interrupt_09h
    push 09h * 4
    call install_int
    add sp, 4
    
    push interrupt_20h
    push 20h * 4
    call install_int
    add sp, 4

    push interrupt_21h
    push 21h * 4
    call install_int
    add sp, 4

    push interrupt_33h
    push 33h * 4
    call install_int
    add sp, 4

    push interrupt_34h
    push 34h * 4
    call install_int
    add sp, 4

    push interrupt_98h
    push 98h * 4
    call install_int
    add sp, 4

	call kernel_main

	cli
	jmp $



install_int:
      push bp
      mov bp, sp
      mov ax, 0
      mov es, ax
      mov si, word [bp + 4]
      mov cx, word [bp + 6]
      mov word[es : si], cx
      mov word[es : si +2], cs
      mov es, ax
      pop bp
      ret

int_08_what_to_play dw 0
sys_int08_arrive dd 0

interrupt_08h:
    cli
    push es
    pusha
    mov al, 1
    xchg al, byte[sys_int08_arrive]

    mov ax, cs
    mov es, ax
    
    mov cx, 0xB800
    mov gs, cx
    mov ah, 0x9

%define INTEVAL 16

    test_up:
    cmp word[es:int_08_what_to_play], INTEVAL
    jge test_left
    mov al, '|'
    inc word[es:int_08_what_to_play]
    jmp test_input_over

    test_left:
    cmp word[es:int_08_what_to_play], INTEVAL * 2
    jge test_right
    mov al, '/'
    inc word[es:int_08_what_to_play]
    jmp test_input_over

    test_right:
    cmp word[es:int_08_what_to_play], INTEVAL * 3
    jge test_down
    mov al, '-'
    inc word[es:int_08_what_to_play]
    jmp test_input_over

    test_down:
    cmp word[es:int_08_what_to_play], INTEVAL * 4
    jge reset_char
    mov al, '\'
    inc word[es:int_08_what_to_play]
    jmp test_input_over

    reset_char:
    mov word[es:int_08_what_to_play], 0

    test_input_over:

    mov word [gs:82],ax

	mov al,0x20
	out 0x20,al
	out 0xA0,al
    popa
    pop es
    sti
	iret

ouchs db 'OUCH!OUCH!'
len_ouch db 10

interrupt_09h:	
    cli
    pusha
    mov cx, 0xB800
    mov gs, cx
	mov bp,bx
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

    ;这里也要切换段（一开始没想到）因为随时可能在用户程序中被调用
    push ds
    push es
    mov ax, cs
    mov ds, ax
    mov es, ax
    call dword kb_handler
    pop es
    pop ds
	mov al,0x20
	out 0x20,al
	out 0xA0,al
    popa
	sti
	iret

interrupt_20h:
      mov ah, 4ch
interrupt_21h:
      cmp ah, 4ch
      jnz panic_21h_func_not_impl
      ;因为引发中断压入的寄存器不会被iret返回，所以要手动清栈
      add sp, 6
      jmp dword[0xA00A]
      iret
panic_21h_func_not_impl:
      jmp $

interrupt_98h:
      cli
      cmp ah, 0h
      jnz func_01
func_00:
      call dword sys_getchar
      jmp int_98h_end
func_01:
      cmp ah, 1h
      jnz func_02
      pusha
      push eax
      call dword sys_current_tty_putchar
      add sp, 4
      popa
      jmp int_98h_end
func_02:
      cmp ah, 2h
      jnz func_03
      push es
      pusha
      mov ax, cs
      mov es, ax
      sti
      sleep_loop:
          cmp byte[es:sys_int08_arrive], 0
          jz sleep_loop
          mov ax, 0
          xchg al, byte[es:sys_int08_arrive]
          loop sleep_loop
      popa
      pop es
      jmp int_98h_end

func_03:

int_98h_end:
      iret

interrupt_33h:
    call dword interrupt_33h_c
    iret

interrupt_34h:
    call dword interrupt_34h_c
    iret
