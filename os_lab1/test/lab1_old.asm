; 李新锐 2018-3

org 07c00h					;告知编译器代码将被加载到07c00h处

;数据段
data:
    count   dw delay
    dcount  dw ddelay
    state   db Dn_Rt         
    x       dw 7
    y       dw 0
    xmax    dw 25
    xmin    dw -1
    ymax    dw 80
    ymin    dw -1
    char    db 'A'
    color   db 01h
    myid db 'Lixinrui 15323032'
    Dn_Rt equ 0                  
    Up_Rt equ 1                  
    Up_Lt equ 2                 
    Dn_Lt equ 3                  
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
    call show
    call move
    call determine_state
    call determine_vx
    call determine_vy
    jmp loop

determine_vx:
    pusha
    mov al, byte[state]
    and al, 02h
    shr al, 1
    mov bl, state
    and bl, 01h
    mov cl, al
    and al, bl
    not cl
    not bl
    and cl, bl
    or  al, cl
    mov byte[vx], cl
    popa
    ret

determine_vy:
    push al
    mov al, byte[state]
    and al, 02h
    shr al, 1
    not al
    mov byte[vy], al
    popa al
    ret
    
determine_state:
    pusha
    mov bx, word[x]
    mov cx, word[y]
    mov al, 0
    cmp al, byte[state]
    jz Det_00
    mov al, 1
    cmp al, byte[state]
    jz Det_01
    mov al, 2
    cmp al, byte[state]
    jz Det_10
    mov al, 3
    cmp al, byte[state]
    jz Det_11
Det_00:
    cmp bx, xmax
    jz To_01
    cmp cx, ymax
    jz To_11
    ret
Det_01:
    cmp bx, xmin
    jz To_00
    cmp cx, ymax
    jz To_10
    ret
Det_10:
    cmp bx, -1
    jz To_11
    cmp cx, -1
    jz To_01
    ret
Det_11:
    cmp cx, ymin
    jz To_00
    cmp bx, xmax
    jz To_10
    ret
To_00:
    mov byte[state], 0
    ret
To_01:
    mov byte[state], 1
    ret
To_10:
    mov byte[state], 2
    ret
To_11:
    mov byte[state], 3
    ret
    
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

change_color:
    cmp byte[color],0Fh ;当前字符颜色是否为最后一种
    jnz no_rst          ;如果不是，选择下一种
    mov byte[color],0   ;如果是，重置
no_rst:
    inc byte[color]     ;选择下一种
    ret
    
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

move:
    call sleep
update_x:
    test vx
    jz sub_x
add_x:
    add byte[x], 1
    jmp update_y
sub_x:
    sub byte[x], 1
update_y:
    test vy
    jz sub_y
    add byte[y], 1
    ret
sub_y:
    sub byte[y], 1
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
	
end:
    jmp $                   ; 停止画框，无限循环 
	


times 510 - ($ -$$)     db  0
dw    0xaa55
