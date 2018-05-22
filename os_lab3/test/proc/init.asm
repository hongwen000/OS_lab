align 4

[bits 32]
[section .text]
[global __init_start]
[global __init_end]


__init_start:
    jmp begin
    shared_v db 'V'
    begin:
    nop
    ;fork
    mov ah, 7
    int 0x98    ; a free process is necessary
    cmp eax, 0
    jz child
    mov ah, 5
    int 0x98
    mov ah, 1
    mov al, byte[shared_v - $$ + 0xc0000000]
    int 0x98
    jmp $

child:
    ;push (msg_argv - $$) + 0xc0000000   ; arguments
    ;push (msg_init - $$) + 0xc0000000   ; path
    ;push 0  ; eip
    ;mov eax, 7  ; exec
    inc byte[shared_v - $$ + 0xc0000000]
    mov ah, 1
    mov al, byte[shared_v - $$ + 0xc0000000]
    int 0x98
    mov ah, 6
    int 0x98

msg_init:
    db "/bin/cinit", 0
; char **argv = {"/bin/cinit", 0}
msg_argv:
    dd (msg_init - $$) + 0xc0000000
    dd 0

__init_end:
