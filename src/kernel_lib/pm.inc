
; kernel code segment selector
SEL_KERN_CODE   EQU 0x8
; kernel data segment selector
SEL_KERN_DATA   EQU 0x10
; vedio memory
SEL_KERN_VIDEO  EQU 0x18
; stack memroy
SEL_KERN_STACK  EQU 0x20
USER_BASE       EQU 0xc0000000
USER_TEXT_BASE  EQU USER_BASE + 0x1000

%define USER_LOAD_ADDR 0x00A100
%define LEN_PSP 0x100
%define USER_CODE_SEL  SEL_KERN_CODE
%define USER_DATA_SEL  SEL_KERN_DATA
%define OPCODE_INT 0xCD
%define PSP_USER_RETURN_ADDR    USER_LOAD_ADDR - LEN_PSP + 0x0A
%define PSP_USER_RETURN_SEG     USER_LOAD_ADDR - LEN_PSP + 0x0A + 4
%define PSP_USER_RETURN_ESP     USER_LOAD_ADDR - LEN_PSP + 0x0A + 8

%macro __ISR_BEGIN_SAFE_ASM__ 0
        pusha
        push ds
        push es
        push fs
        push gs
        push ax

        mov ax, SEL_KERN_DATA
        mov ds, ax
        mov es, ax
;        mov ss, ax
        mov fs, ax
;        mov ss, ax
        mov ax, SEL_KERN_VIDEO
        mov gs, ax
        pop ax
%endmacro

%macro __ISR_LEAVE_SAFE_ASM__ 0
        pop gs
        pop fs
        pop es
        pop ds
        popa
%endmacro

%define __AX__STUB__ 0x2c

%macro __ACTIVE_PCI__ 0
	mov al,0x20
	out 0x20,al
	out 0xA0,al
%endmacro

%define PROC_STAT_NOT_READY 0
%define PROC_STAT_READY 1
%define PROC_STAT_RUNNING 2
%define PROC_STAT_WAITING 3
