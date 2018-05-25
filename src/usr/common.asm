; 李新锐 2018-3
[bits 32]

%macro putchar 4
    pusha
    xor ax,ax                 ; 计算显存地址
    mov ax,%1
	mov bx,80
	mul bx
	add ax,%2
	mov bx,2
	mul bx
	mov bp,ax
	mov ah,%3			;  0000：黑底、1111：亮白字（默认值为07h）
	mov al,%4			;  AL = 显示字符值（默认值为20h=空格符）
	mov word[gs:bp],ax  		;  显示字符的ASCII码值
    ;call print_id
    popa
%endmacro

%define len         20
%define sizeof_word 2

%macro program 4

SECTION .text align=4096
[global main]
main:
    ;call cls                    ; 清除BIOS显示的信息
    ;mov ax,cs
	;mov es,ax					; ES = 0
	;mov ds,ax					; DS = CS
	;mov es,ax					; ES = CS
;	mov	ax,0B800h				; 文本窗口显存起始地址
;	mov	gs,ax					; GS = B800h
    mov byte[char],'A'
    init_x:
    mov ax, word[xmin]
    cmp word[x], ax
    jg  init_y
    inc ax
    mov word[x], ax
    init_y:
    mov ax, word[ymin]
    cmp word[y], ax
    jg  _loop
    inc ax
    mov word[y], ax

    _loop:
    call move
    call change_speed
    ;putchar word[old_x], word[old_y], 0x0000, 0x20
    putchar word[x], word[y], byte[color], byte[char]
    inc word[cnt]
    ;call record_histroy
    mov cx, 600
    OUTER:
    mov bx, 100
    INNER:
    dec bx
    jg INNER
    and cx, 0xFFF
    loop OUTER

    jmp _loop

record_histroy:
    pusha
    cld
    mov esi, old_x + 1 * sizeof_word
    mov edi, old_x
    %rep    len - 1
            movsw
    %endrep
    mov esi, old_y + 1 * sizeof_word
    mov edi, old_y
    %rep    len - 1
            movsw
    %endrep
    mov ax, word[x]
    mov word[old_x + (len - 1) * sizeof_word], ax
    mov ax, word[y]
    mov word[old_y + (len - 1) * sizeof_word], ax
    popa
    ret


move:
    pusha
    mov ax, word[vx]
    mov bx, word[vy]
    add word[x], ax
    add word[y], bx
    popa
    ret

change_color:

    cmp byte[color],0Fh ;当前字符颜色是否为最后一种
    jnz no_rst          ;如果不是，选择下一种

    mov    esi,%1
    jmp    .l1
    .l4
    mov    ecx,%3
    jmp    .l2
    .l3
    inc    ecx
    .l2

        mov ax, si
        mov bx, 80
        mul bx
        add ax, cx
        shl ax, 1
        mov bp, ax
        mov ah, 0x7
        mov al, ' '
        mov word[gs:bp], ax

    cmp    ecx,%4
    jle    .l3
    inc    esi
    .l1:
    cmp    esi,%2
    jle    .l4
    mov byte[color],0   ;如果是，重置
no_rst:
    inc byte[color]     ;选择下一种
    ret

change_speed:
    pusha
check_x:
    mov ax, word[x]
    cmp ax, word[xmin]
    jz  reverse_vx
    cmp ax, word[xmax]
    jz  reverse_vx
    jmp check_y
reverse_vx:
    call change_color
    neg word[vx]
check_y:
    mov bx, word[y]
    cmp bx, word[ymin]
    jz reverse_vy
    cmp bx, word[ymax]
    jz reverse_vy
    popa
    ret
reverse_vy:
    call change_color
    neg word[vy]
    popa
    ret

    
    
end:
    jmp $                   ; 停止画框，无限循环 
	
;数据段

SECTION .data align=4096
    count   dw delay
    dcount  dw ddelay
    x       dw 7 + %1 + 1
    y       dw 0 + %3 + 1
    xmin    dw %1
    xmax    dw %2
    ymin    dw %3
    ymax    dw %4
    vx      dw 1
    vy      dw 1
    char    db 'A'
    color   db 01h
    myid    db 'Lixinrui 15323032'
    old_x   times len   dw 0
    old_y   times len   dw 0
    cnt     dw 0
    delay equ 60
    ddelay equ 100


SECTION .bss align=4096

%endmacro
