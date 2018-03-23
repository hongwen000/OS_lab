;[section .bss]
;align 16
;[section .stack]
;stack_bottom:
;times 16384 db 0
;stack_top:
[BITS 16]
[section .text]
[global kernel_start]
[extern _init]
[extern kernel_main]
kernel_start:

	call _init

	call kernel_main

	cli
	jmp $