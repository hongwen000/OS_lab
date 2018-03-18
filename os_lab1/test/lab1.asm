; 李新锐 2018-3

org 07c00h					;告知编译器代码将被加载到07c00h处

jmp main

;数据段
data:
    count   dw delay
    dcount  dw ddelay
    x       dw 7
    y       dw 0
    xmax    dw 25
    xmin    dw -1
    ymax    dw 80
    ymin    dw -1
    vx      dw 1
    vy      dw 1
    char    db 'A'
    color   db 01h
    myid db 'Lixinrui 15323032'
    delay equ 6000					
    ddelay equ 1000					

main:
    call cls                    ; 清除BIOS显示的信息
    mov ax,cs
	mov es,ax					; ES = 0
	mov ds,ax					; DS = CS
	mov es,ax					; ES = CS
	mov	ax,0B800h				; 文本窗口显存起始地址
	mov	gs,ax					; GS = B800h
    mov byte[char],'A'
loop:
    call sleep
    call show
    call move
    call change_speed
    jmp loop

cls:
    pusha           ;保存寄存器的值
    mov ah,0x06     ;调用10号BIOS中断的6号功能
    mov al,0        ;al=0代表清屏
    mov bh,0x07     ;设置将屏幕置为黑底白字
    mov ch,0        ;从(0,0)到(24,79)
    mov cl,0   
    mov dh,24  
    mov dl,79  
    int 0x10        ;调用中断
    popa            ;恢复寄存器的值
    ret             ;返回

sleep:
    pusha
    mov cx, ddelay      
    OUTER:
        mov bx, delay
        INNER:
            dec bx
            jg INNER
    loop OUTER
    popa
    ret

show:	
    pusha
    xor ax,ax                 ; 计算显存地址
    mov ax,word[x]
	mov bx,80
	mul bx
	add ax,word[y]
	mov bx,2
	mul bx
	mov bp,ax
	mov ah,byte[color]			;  0000：黑底、1111：亮白字（默认值为07h）
	mov al,byte[char]			;  AL = 显示字符值（默认值为20h=空格符）
    call cls
	mov word[gs:bp],ax  		;  显示字符的ASCII码值
    call print_id
    popa
    ret

print_id:
    pusha
    mov ax, myid 
    mov bp, ax      ;es:bp: 字符串首地址
    mov cx, 17      ;字符串长度
    mov ax, 01300h  ;调用Write string功能
    mov bx, 00F1h   ;白底蓝字，闪烁
    mov dx, 00920h  ;显示在屏幕中央
    int 10h
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
	
times 510 - ($ -$$)     db  0
dw    0xaa55
