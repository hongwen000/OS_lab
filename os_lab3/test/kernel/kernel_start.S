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


kernel_start:
    push interrupt_20h
    push 20h * 4
    call install_int
    add sp, 4

    push interrupt_21h
    push 21h * 4
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