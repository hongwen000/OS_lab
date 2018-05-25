; 李新锐 2018-3

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
%define org_len

org 0A100h					;告知编译器代码将被加载到07c00h处

jmp main

;数据段
data:
    count   dw delay
    dcount  dw ddelay
    x       dw 12
    y       dw 40
    fruit_x dw 0
    fruit_y dw 0
    cur_len dw 2
    xmin    dw -1
    xmax    dw 25
    ymin    dw -1
    ymax    dw 80
    vx      dw 1
    vy      dw 0
    char    db 'A'
    color   db 03h
    readme  db 'Play: press w/a/s/d to go up/left/down/right'
    rd_len  db $-readme
    old_x   times len   dw 0
    old_y   times len   dw 0
    cnt     dw 0
    delay equ 600					
    ddelay equ 100					
    is_over dw 0

main:
    call cls                    ; 清除BIOS显示的信息
    mov ax,cs
	mov es,ax					; ES = 0
	mov ds,ax					; DS = CS
	mov es,ax					; ES = CS
	mov	ax,0B800h				; 文本窗口显存起始地址
	mov	gs,ax					; GS = B800h
    mov byte[char],'*'
    call make_new_fruit
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
        mov cx, ddelay      
        OUTER:
            mov bx, delay
            INNER:
                xor ax, ax
                mov ah, 01h
                int 16h
                jz test_input_over   ;无按键按下
                mov dl, al  ;dl<-按键ASCII码
                mov ah, 00h ;清除键盘缓冲区
                int 16h     ;清除键盘缓冲区
                test_exit:
                cmp dl, 27
                jnz test_up
                jmp exit
                test_up:
                cmp dl, 119
                jnz test_left
                mov word[vx], -1
                mov word[vy], 0
                jmp test_input_over
                test_left:
                cmp dl, 97
                jnz test_right
                mov word[vx], 0
                mov word[vy], -1
                jmp test_input_over
                test_right:
                cmp dl, 100
                jnz test_down
                mov word[vx], 0
                mov word[vy], 1
                jmp test_input_over
                test_down:
                cmp dl, 115
                jnz test_input_over
                mov word[vx], 1
                mov word[vy], 0
                test_input_over:
                dec bx
                jg INNER
        loop OUTER

    cmp word[is_over], 1
    jz  _loop

    call move
    mov  ax, word[x]
    mov  bx, word[y]
    cmp  ax, word[fruit_x]
    jnz  not_have_eaten
    cmp  bx, word[fruit_y]
    jnz  not_have_eaten
    putchar word[fruit_x], word[fruit_y], 0x0000, 0x20
    inc  word[cur_len]
    call make_new_fruit

not_have_eaten:
    call change_speed
    putchar word[old_x], word[old_y], 0x0000, 0x20
    putchar word[x], word[y], byte[color], byte[char]
    inc word[cnt]
    call record_histroy
    call print_id
    jmp _loop

make_new_fruit:
    pusha
    push 18             ;参数压栈
    call rand
    add  dx, 4          ;dx = rand() % 18 + 2
    mov word[fruit_x], dx
    pop  dx
    push 70
    call rand
    add  dx, 2          ;dx = rand() % 60 + 2
    mov word[fruit_y], dx
    pop  dx
    putchar word[fruit_x], word[fruit_y], 0x00F1, 'o'
    popa
    ret

rand:
    push bp                 ;保存bp
    mov  bp, sp             ;设置bp为栈顶
    mov  ah, 0
    int  1ah
    and  dx, 0x11ee           ;防止除法溢出
    mov  ax, dx             ;dx读出时钟的值
    mov  dx, word[bp + 4]   ;通过bp读取参数
    div  dl                 ;ah <- ax % dl
    mov  dl, ah             ;dl <- ah
    mov  dh, 0
    mov  sp, bp             ;清理局部变量（此处没用到，但是是函数调用通用的要求）
    pop  bp                 ;恢复bp
    ret

record_histroy:
    pusha
    cld
    mov si, old_x + 1 * sizeof_word
    mov di, old_x
    mov cx, word[cur_len]
    dec cx
    movsw1:
            movsw
            loop movsw1
    mov si, old_y + 1 * sizeof_word
    mov di, old_y
    mov cx, word[cur_len]
    dec cx
    movsw2:
            movsw
            loop movsw2
    mov di, word[cur_len]
    dec di
    add di, di
    mov ax, word[x]
    mov word[old_x + di], ax
    mov ax, word[y]
    mov word[old_y + di], ax
    popa
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
    mov ax, readme 
    mov bp, ax      ;es:bp: 字符串首地址
    mov cl, byte[rd_len]      ;字符串长度
    mov ax, 1300h  ;调用Write string功能
    mov bx, 000Fh   ;
    mov dx, 0000h  ;
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

game_over:
    mov word[is_over], 1
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
    call game_over
check_y:
    mov bx, word[y]
    cmp bx, word[ymin]
    jz reverse_vy
    cmp bx, word[ymax]
    jz reverse_vy
    popa
    ret
reverse_vy:
    call game_over
    popa
    ret

exit:
    mov ah, 4ch
    int 21h
	
times 1024 - ($ -$$)     db  0

