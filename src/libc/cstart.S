[BITS 32]
[section .text]
[global _start]
[extern main]
_start:
    call main
	mov ah, 6
	int 0x98

