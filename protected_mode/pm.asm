%include "pm.inc"
org 0x7c00
jmp begin
lable_gdt:          Descriptor               0,                   0,   0
lable_desc_code32:  Descriptor    GDT_UNDEFINE,  seg_code32_len - 1,   DA_C + DA_32
lable_desc_video:   Descriptor         0xB8000,              0xffff,   DA_DRW

gdt_len     equ     $ - lable_gdt
gdt_ptr     dw      gdt_len - 1
            dd      GDT_UNDEFINE

selector_code32     equ     lable_desc_code32 - lable_gdt
selector_video      equ     lable_desc_video  - lable_gdt

[BITS 16]
begin:
    mov ax, cs
    mov ds, ax
    mov es, ax
    mov ss, ax

    ;由于nasm的限制，只好在这里设置begin32
    xor eax, eax
    mov ax, cs
    ;cs * 10h + begin32
    shl eax, 4
    add eax, begin32
    ;+2存放低16位
    mov word [lable_desc_code32 + 2], ax
    shr eax, 16
    ;+4存放17-24位
    mov byte [lable_desc_code32 + 4], al
    ;+7存放25-32位
    mov byte [lable_desc_code32 + 7], ah

    ;设置gdt_ptr指向lable_gdt
    xor eax, eax
    mov ax, ds
    shl eax, 4
    add eax, lable_gdt
    mov dword [gdt_ptr + 2], eax

    lgdt [gdt_ptr]
    cli
    in al, 92h
    or al, 00000010b
    out 92h, al
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp dword selector_code32:0

[BITS 32]
begin32:
    mov ax, selector_video
    mov gs, ax
    mov edi, 0
    mov ah, 0x0C
    mov al, 'P'
    mov [gs:edi], ax
    jmp $
seg_code32_len      equ     $ - begin32

times 510-($-$$) db 0
db 0x55,0xaa
