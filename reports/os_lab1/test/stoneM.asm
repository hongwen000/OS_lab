; Version 1.1
; 李新锐 2018-2
;
; Original Version
; 程序源代码（stone.asm）
; 本程序在文本方式显示器上从左边射出一个*号,以45度向右下运动，撞到边框后反射,如此类推.
; 凌应标 2014/3
; NASM汇编格式

data:
    Dn_Rt equ 1                  ;D-Down,U-Up,R-right,L-Left
    Up_Rt equ 2                  ;
    Up_Lt equ 3                  ;
    Dn_Lt equ 4                  ;
    delay equ 5000					; 计时器延迟计数,用于控制画框的速度
    ddelay equ 1000					; 计时器延迟计数,用于控制画框的速度
    count   dw delay
    dcount  dw ddelay
    rdul    db Dn_Rt         ; 向右下运动
    x       dw 7
    y       dw 0
    char    db 'L'
    color   db 01h
    myid db 'Lixinrui 15323032'

org 07c00h					; 程序加载到100h，可用于生成COM

start:
    call cls                    ; 清除BIOS显示的信息
    mov ax,cs
	mov ds,ax					; DS = CS
	mov es,ax					; ES = CS
	mov	ax,0B800h				; 文本窗口显存起始地址
	mov	gs,ax					; GS = B800h
    mov byte[char],'L'


move:
    call sleep
      mov al,1
      cmp al,byte[rdul]
	jz  DnRt
      mov al,2
      cmp al,byte[rdul]
	jz  UpRt
      mov al,3
      cmp al,byte[rdul]
	jz  UpLt
      mov al,4
      cmp al,byte[rdul]
	jz  DnLt
      jmp $	

DnRt:
	inc word[x]
	inc word[y]
	mov bx,word[x]
	mov ax,25
	sub ax,bx
      jz  dr2ur
	mov bx,word[y]
	mov ax,80
	sub ax,bx
      jz  dr2dl
	jmp show
dr2ur:
      mov word[x],23
      mov byte[rdul],Up_Rt	
    call change_color
      jmp show
dr2dl:
      mov word[y],78
      mov byte[rdul],Dn_Lt	
    call change_color
      jmp show

UpRt:
	dec word[x]
	inc word[y]
	mov bx,word[y]
	mov ax,80
	sub ax,bx
      jz  ur2ul
	mov bx,word[x]
	mov ax,-1
	sub ax,bx
      jz  ur2dr
	jmp show
ur2ul:
      mov word[y],78
      mov byte[rdul],Up_Lt	
    call change_color
      jmp show
ur2dr:
      mov word[x],1
      mov byte[rdul],Dn_Rt	
    call change_color
      jmp show

	
	
UpLt:
	dec word[x]
	dec word[y]
	mov bx,word[x]
	mov ax,-1
	sub ax,bx
      jz  ul2dl
	mov bx,word[y]
	mov ax,-1
	sub ax,bx
      jz  ul2ur
	jmp show

ul2dl:
      mov word[x],1
      mov byte[rdul],Dn_Lt	
      call change_color
      jmp show
ul2ur:
      mov word[y],1
      mov byte[rdul],Up_Rt	
      call change_color
      jmp show

	
	
DnLt:
	inc word[x]
	dec word[y]
	mov bx,word[y]
	mov ax,-1
	sub ax,bx
      jz  dl2dr
	mov bx,word[x]
	mov ax,25
	sub ax,bx
      jz  dl2ul
	jmp show

dl2dr:
      mov word[y],1
      mov byte[rdul],Dn_Rt	
      call change_color
      jmp show
	
dl2ul:
      mov word[x],23
      mov byte[rdul],Up_Lt	
      call change_color
      jmp show
	
show:	
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
	jmp move
	
end:
    jmp $                   ; 停止画框，无限循环 

cls:
    pusha
    mov ah,0x06
    mov al,0   
    mov bh,0x07
    mov ch,0   
    mov cl,0   
    mov dh,24  
    mov dl,79  
    int 0x10
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

change_color:
    cmp byte[color],0Fh
    jnz no_rst
    mov byte[color],0
no_rst:
    inc byte[color]
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

times 510 - ($ -$$)     db  0
dw    0xaa55
