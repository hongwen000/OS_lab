%INCLUDE "boot/pm.asm"
[BITS 16]
org    0x7c00        

jmp entry

%define KERNEL_ENTRY_ADDR 0x00008000

temp_print16:
loop:
    lodsb   ; ds:si -> al
    or al,al
    jz done 
    mov ah,0x0e        
    mov bx,15        
    int 0x10        
    jmp loop
done:
    ret

;============================================================
entry:
    mov ax,0        
    mov ss,ax
    mov sp,0x7c00
    mov ds,ax
    mov es,ax   ; bios interrupt expects ds

    ; shift to text mode, 16 color 80*25
    mov ah,0x0
    mov al,0x03 ; 
    int 0x10

    mov si, msg_boot
    call temp_print16

;============================================================
; get memory info 
ADRS equ 0x500             ;储存"地址范围描述符"的地址
MCR_count equ 0x400        ;储存"内存分段数计数器"的地址
get_mem_map:
    mov dword [MCR_count], 0
    mov ax, 0
    mov es, ax
    xor ebx, ebx
    mov di,ADRS            ; ADRS将被写入到该地址

get_mem_map_loop:
    mov eax, 0xe820        ; Traverse all memory
    mov ecx, 20            ; ARDS 的大小
    mov edx, 0x534D4150    ; edx = "SMAP"
    int 0x15
    jc get_mem_map_fail    ;int 15设置cf则失败了
    add di, 20
    inc dword [MCR_count]  ;分段数加一
    cmp ebx, 0             ;如果内存区域扫描完毕，则ebx == 0
    jne get_mem_map_loop 
    jmp get_mem_map_ok

get_mem_map_fail:
    mov si,msg_get_mem_map_err
    call temp_print16
    hlt

get_mem_map_ok:

;============================================================
; read kernel form disk
; NB: 1.44 MB 软盘的最大连续扇区只有 18*2 = 36
; 而 bochs 对连续读取扇区的长度限制是 72 2.88 M 软盘的最大连续扇区数 36*2
; http://stackoverflow.com/questions/3645803/bios-int-13h-with-ah-2-can-only-read-72-sectors-each-time-why
; http://f.osdev.org/viewtopic.php?f=13&t=22491

; read 20 cylinder (360 KB) form floppy (暂定)
loadloader:      
    mov bx, KERNEL_ENTRY_ADDR    
    mov ax,0 
    mov es,ax   ; es:bx buffer address point -> 0x8000
    mov ch,0    ; cylinder
    mov dh,0    ; head
    mov cl,2    ; sector
    mov dl,0x00 ; driver a:
; kernel locates after bootloader, which is the second sector

readloop:
    mov si,0    ; err counter 

retry:
    mov ah,0x02 ; int 0x13  ah = 0x02 read sector form dirve
    mov al, 1   ; read 1 sector 
    int 0x13
;     jnc next    ;cf is set when error occur
;     add si,1    ;error counter + 1
;     cmp si,5    
;     jae error
;     mov ah,0x00
;     mov dl,0x00 ; driver a
;     int 0x13    ; reset
;     jmp next 

; next: 
    mov ax, es
    add ax, 0x20    ; 0x200 size of a sector
    mov es, ax  ; write address += 0x200

    add cl, 1   ; sector + 1
    cmp cl, 18  ; 18 sector
    jbe readloop

    mov cl, 1
    add dh, 1   ; head <- 1
    cmp dh, 1
    jbe readloop

    mov dh, 0   ; head <- 0
    add ch, 1   ; cylinder + 1
    cmp ch, 5
    jbe readloop
    ; jmp succ

; error:        
;     mov  si,msg_err
;     call temp_print16
;     jmp $

; succ:    
    mov si,msg_succ
    call temp_print16

    ; fill and load GDTR
    xor eax,eax
    mov ax,ds
    shl eax,4
    add eax,GDT        ; eax <- gdt base 
    mov dword [GdtPtr+2],eax    ; [GdtPtr + 2] <- gdt base 

    lgdt [GdtPtr]
    cli

    mov si,msg_gdt
    call temp_print16

    ; turn on A20 line
    in al,0x92   
    or al,00000010b
    out  0x92, al

    mov si,msg_a20
    call temp_print16


    ; shift to protectmode  
    mov eax,cr0
    or eax, 1
    mov cr0,eax

    ; special, clear pipe-line and jump 
    jmp dword Selec_Code32_R0:KERNEL_ENTRY_ADDR

msg_boot:
    db "Bootsector loaded...",13,10,0
msg_err:
    db "Loader load error.",13,10,0
msg_succ:
    db "Loader load success...",13,10,0
msg_gdt:
    db "Temp GDT loaded...",13,10,0
msg_a20:
    db "A20 line on...",13,10,0
msg_temp:
    db 0,0,0
msg_get_mem_map_err:
    db "Get memory message error, system halt.", 0

GDT:
DESC_NULL:        Descriptor 0,       0,            0             ; null
DESC_CODE32_R0:   Descriptor 0,       0xfffff - 1,  DA_C+DA_32+DA_LIMIT_4K    ; uncomfirm 
DESC_DATA_R0:     Descriptor 0,       0xfffff - 1,  DA_DRW+DA_32+DA_LIMIT_4K  ; uncomfirm  ; 4G seg 
DESC_VIDEO_R0:    Descriptor 0xb8000, 0xffff,       DA_DRW+DA_32  ; vram 

GdtLen  equ $ - GDT     ; GDT len
GdtPtr  dw  GdtLen - 1  ; GDT limit
        dd  0           ; GDT Base

; GDT Selector 
Selec_Code32_R0 equ     DESC_CODE32_R0 - DESC_NULL
Selec_Data_R0   equ     DESC_DATA_R0   - DESC_NULL 
Selec_Video_R0  equ     DESC_VIDEO_R0  - DESC_NULL

times 510 - ($-$$) db 0
db 0x55, 0xaa

