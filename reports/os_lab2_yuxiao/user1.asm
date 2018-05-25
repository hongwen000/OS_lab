;如有移动过快或过慢问题，请修改delay与delay_t的值

D_R equ 1
U_R equ 2
U_L equ 3
D_L equ 4
delay_t equ 5
delay equ 30

start:
	;设定数据段偏移量
	mov ax,cs
	;mov ds,ax
	;设定显存偏移量
	mov ax,0b800h
	mov es,ax
	call near clean
timing:
    ;退出判断
    ;mov ah,02h
    ;int 16h
    ;cmp al,0ch
    ;jz ret_to_os
	;设定延迟
	dec word [count]
	jnz timing
	;设定延迟倍数
	mov word[count],delay
	dec word [delay_t]
	jnz timing
	mov word [count],delay
	mov word [count_t],delay_t
	;初始化计数器
	mov si,message_pg1
	xor di,di
	inc di
	inc di
	mov cx,messagelen
personal:
	;显示个人信息
	mov al,[si]
	mov ah,[color]
	mov byte [es:di],al
	inc si
	inc di
	mov byte [es:di],ah
	inc di
	loop personal
	

;判断下一次移动方向
judging:
	mov al,D_R
	cmp al,[direction]
	jz Dn_Rt
	mov al,U_R
	cmp al,[direction]
	jz Up_Rt
	mov al,U_L
	cmp al,[direction]
	jz Up_Lt
	mov al,D_L
	cmp al,[direction]
	jz Dn_Lt
;Moving
Dn_Rt:
	inc word [x]
	inc word [y]
	;判断是否撞到底部
	mov ax,13
	sub ax,[x]
	jz Dr2Ur
	;判断是否撞到右侧
	mov ax,40
	sub ax,[y]
	jz Dr2Dl
	jmp display
Up_Rt:
	dec word [x]
	inc word [y]
	;判断是否撞到顶部
	mov ax,-1
	sub ax,[x]
	jz Ur2Dr
	;判断是否撞到右侧
	mov ax,40
	sub ax,[y]
	jz Ur2Ul
	jmp display
Up_Lt:
	dec word [x]
	dec word [y]
	;判断是否撞到顶部
	mov ax,-1
	sub ax,[x]
	jz Ul2Dl
	;判断是否撞到左侧
	mov ax,-1
	sub ax,[y]
	jz Ul2Ur
	jmp display
Dn_Lt:
	inc word [x]
	dec word [y]
	;判断是否撞到底部
	mov ax,13
	sub ax,[x]
	jz Dl2Ul
	;判断是否撞到左侧
	mov ax,-1
	sub ax,[y]
	jz Dl2Dr
	jmp display
;改变运动方向
Dr2Ur:
	mov word [x],11
	mov byte [direction],U_R
	call near clean
	jmp display
Dr2Dl:
	mov word [y],38
	mov byte [direction],D_L
	call near clean
	jmp display
Ur2Ul:
	mov word [y],38
	mov byte [direction],U_L
	call near clean
	jmp display
Ur2Dr:
	mov word [x],1
	mov byte [direction],D_R
	call near clean
	jmp display
Ul2Dl:
	mov word [x],1
	mov byte [direction],D_L
	call near clean
	jmp display
Ul2Ur:
	mov word [y],1
	mov byte [direction],U_R
	call near clean
	jmp display
Dl2Dr:
	mov word [y],1
	mov byte [direction],D_R
	call near clean
	jmp display
Dl2Ul:
	mov word [x],11
	mov byte [direction],U_L
	call near clean
	jmp display
;显示模块
display:
	;计算显存地址
	mov ax,[x]
	mov bx,80
	mul bx
	add ax,[y]
	mov bx,2
	mul bx
	mov bx,ax
	;将字符装入显存
	mov byte ah,[color]
	mov byte al,[char]
	mov [es:bx],ax
	;改变颜色
	inc byte [color]
	;jmp timing
;清屏
clean:
	mov cx,07FFFh
	xor bx,bx
cln:
	mov byte [es:bx],0
	inc bx
	loop cln
	ret
ret_to_os:
    ret
datadef:
	count_t dw delay_t
	count dw delay
	direction db D_R
	x dw 0
	y dw 0
	char db 'W'
	color db 1
	message_pg1 db '16337328ZhengYuXiao'
	messagelen equ $ - message_pg1
	
	
