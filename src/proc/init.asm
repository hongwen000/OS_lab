%include "kernel_lib/pm.inc"
align 4

[bits 32]
[section .text]
[global __init_start]
[global __init_end]


__init_start:
    nop
    ;fork
    mov ah, 4
    int 0x98    ; a free process is necessary
    cmp eax, 0
    jz child
    ; Because currently kernel is not reentrant, there must be a running process
    jmp $

child:
    ;push (shell - $$) + 0xc0002000   ; path
    ;;push 192
    ;push 0
    ;mov ah, 8
    ;int 0x98
    ;add esp, 8
    jmp $

shell:
    db "/SH.ELF", 0
__init_end:
