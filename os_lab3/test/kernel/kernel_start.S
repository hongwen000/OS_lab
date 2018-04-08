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
[extern sys_current_tty_putchar]
[extern system_call_getchar]
[extern fun_in_c]
[global sys_int08_arrive]

kernel_start:

    mov bh, 3
    cli
    mov al, 0x8A
    out 0x70, al
    in al, 0x71
    and al, 0xF0
    or bh, al
    mov al, 0x8A
    out 0x70, al
    mov al, bh
    out 0x71, al
    sti
    push interrupt_08h
    push 08h * 4
    call install_int
    add sp, 4

	mov ax, [09h * 4]
	mov word [int_09_saved], ax
	mov ax, [09h * 4 + 2]
	mov word [int_09_saved + 2], ax

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

int_08_what_to_play db 0
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

    test_up:
    cmp byte[es:int_08_what_to_play], 0
    jnz test_left
    mov al, '|'
    mov byte[es:int_08_what_to_play], 1
    jmp test_input_over
    test_left:
    cmp byte[es:int_08_what_to_play], 1
    jnz test_right
    mov al, '/'
    mov byte[es:int_08_what_to_play], 2
    jmp test_input_over
    test_right:
    cmp byte[es:int_08_what_to_play], 2
    jnz test_down
    mov al, '-'
    mov byte[es:int_08_what_to_play], 3
    jmp test_input_over
    test_down:
    cmp byte[es:int_08_what_to_play], 3
    jnz test_input_over
    mov al, '\'
    mov byte[es:int_08_what_to_play], 0
    test_input_over:

    mov word [gs:0x500],ax

	mov al,0x20
	out 0x20,al
	out 0xA0,al
    popa
    pop es
    sti
	iret

int_09_saved dd 0
ouchs db 'OUCH!OUCH!'
len_ouch db 10

interrupt_09h:	

    pusha
    mov cx, 0xB800
    mov gs, cx
	mov bp,bx
    mov ah, 0x9
    mov al, 'O'
    mov word [gs:0],ax
    mov al, 'U'
    mov word [gs:2],ax
    mov al, 'C'
    mov word [gs:4],ax
    mov al, 'H'
    mov word [gs:6],ax
    mov al, 'S'
    mov word [gs:8],ax
    mov al, '!'
    mov word [gs:10],ax
    mov al, 'O'
    mov word [gs:12],ax
    mov al, 'U'
    mov word [gs:14],ax
    mov al, 'C'
    mov word [gs:16],ax
    mov al, 'H'
    mov word [gs:18],ax
    mov al, '!'
    mov word [gs:20],ax
    popa

	push es
	push ax
	mov ax, cs
	mov es, ax
	sti
	pushf
	call far [es:int_09_saved]

	pop ax
	pop es
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
      cmp ah, 0h
      jnz func_01
func_00:
      call sys_bios_getchar
      jmp int_98h_end
func_01:
      pusha
      cmp ah, 1h
      jnz func_02
      push eax
      call dword sys_current_tty_putchar
      add sp, 4
      popa
      jmp int_98h_end
func_02:

int_98h_end:
      iret
