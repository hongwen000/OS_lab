%include "kernel_lib/pm.inc"

align 4

[bits 32]
[section .text]

[extern kernel_pm_setup]
[global kernel_start]
kernel_start:
    xor eax, eax
    mov ax, SEL_KERN_DATA
    mov ds, ax
    mov ax, SEL_KERN_DATA
    mov es, ax
    mov ax, SEL_KERN_VEDIO
    mov gs, ax
    mov ax, SEL_KERN_DATA
    mov ss, ax
    mov esp, 0x7c00

; mov the kernel to 0x100000
[extern kernstart]
[extern kernend]
    mov eax, kernend
    mov ecx, kernstart
    sub eax, ecx
    mov ecx, eax
    mov esi, 0x8000
    mov edi, 0x100000
    cld
    rep movsb
    jmp dword SEL_KERN_CODE:go

go:
    mov	edi, (160*3)+0   ; 160*50 line 3 column 1 
    mov	ah, 00001100b    ; red  

    
    mov esi, msg_pm  
    call temp_print32 

    [extern gdt_init]
    [extern idt_init]
    call gdt_init
    call idt_init
    mov	edi, (160*4)+0   ; 160*50 line 3 column 1 
    mov	ah, 00001100b    ; red  
    mov esi, msg_init_ok
    call print32
    jmp $

    cli
    hlt


temp_print32:
    add edi, 160
    push edi
    cld

loop:
    lodsb
    cmp al, 0
    je outloop
    mov	[gs:edi], ax
    add edi, 2
    jmp loop

outloop:
    pop edi
    ret

msg_pm:
    db "In protect mode now.", 0

print32:
    add edi, 160
    push edi
    cld

.loop:
    lodsb
    cmp al, 0
    je .outloop
    mov	[edi + 0xB8000], ax
    add edi, 2
    jmp .loop

.outloop:
    pop edi
    ret
msg_init_ok:
    db "GDT and IDT init OK", 0