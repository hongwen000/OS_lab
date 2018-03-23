;[section .bss]
;align 16
;[section .stack]
;stack_bottom:
;times 16384 db 0
;stack_top:
[BITS 32]
[section .text]
[global kernel_start]
[extern _init]
[extern kernel_main]
kernel_start:

	mov ax, 0
	mov fs, ax

	;call  dword far [fs: word 0x8169]

	call kernel_main

	cli
	jmp $