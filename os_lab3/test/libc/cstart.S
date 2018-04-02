[BITS 16]
[section .text]
[global _start]
[extern main]
_start:
    call dword main
	mov ah, 4ch
	int 21h

