[BITS 16]
[section .text]
[global _start]
[extern main]
_start:

	call main

	mov ah, 4ch
	int 21h

