;程序源代码（myos1.asm）
org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行
OffSetOfUserPrg1 equ 8100h
OffSetOfUserPrg2 equ 8300h
OffSetOfUserPrg3 equ 8500h
OffSetOfUserPrg4 equ 8700h
delay equ 300
delay_t equ 50

Start:
	mov	ax, cs	       ; 置其他段寄存器值与CS相同
	mov	ds, ax	       ; 数据段
	mov	es, ax		 ; 置ES=DS
	mov	bp, Message		 ; BP=当前串的偏移地址
	mov	cx, MessageLength  ; CX = 串长（=9）
    call displayNotice
LoadnEx:
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
      mov ax,cs                ;段地址 ; 存放数据的内存基地址
      mov es,ax                ;设置段地址（不能直接mov es,段地址）
      mov bx, OffSetOfUserPrg1  ;偏移地址; 存放数据的内存偏移地址
      mov ah,2                 ; 功能号
      mov al,4                 ;扇区数
      mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
      mov dh,0                 ;磁头号 ; 起始编号为0
      mov ch,0                 ;柱面号 ; 起始编号为0
      mov cl,2                 ;起始扇区号 ; 起始编号为1
      int 13H ;                调用读磁盘BIOS的13h功能
      ; 用户程序a.com已加载到指定内存区域中
ProgramSelect:
      mov bp,Message_S
      mov cx,Message_S_length
      call displayNotice
      mov ah,0
      int 16h
      cmp al,'a'
      je UserPrg1
      cmp al,'s'
      je UserPrg2
      cmp al,'d'
      je UserPrg3
      cmp al,'f'
      je UserPrg4
UserPrg1:
    ;mov bp,Message_1
    ;mov cx,Message_Noti_length
    ;call near displayNotice
    push ds
    mov ax, 0x8100
    ;mov ds, ax
    call OffSetOfUserPrg1
    pop ax
    ;mov ds, ax
    jmp ProgramSelect
UserPrg2:
    mov bp,Message_2
    mov cx,Message_Noti_length
    call near displayNotice
    call OffSetOfUserPrg2
    jmp ProgramSelect
UserPrg3:
    mov bp,Message_3
    mov cx,Message_Noti_length
    call near displayNotice
    call OffSetOfUserPrg3
    jmp ProgramSelect
UserPrg4:
    mov bp,Message_4
    mov cx,Message_Noti_length
    call near displayNotice
    call OffSetOfUserPrg4
    jmp ProgramSelect
timing:
	;设定延迟
	dec word [count]
	jnz timing
	;设定延迟倍数
	mov word[count],delay
	dec word [delay_t]
	jnz timing
	mov word [count],delay
	mov word [count_t],delay_t
    ret
displayNotice:
    mov al,00h
    mov ah,06h
    int 10h ;清空屏幕
	;mov	bp, Message		 ; BP=当前串的偏移地址
	;mov	cx, MessageLength  ; CX = 串长（=9）
	mov	ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
    mov dh, 0		       ; 行号=0
	mov	dl, 0			 ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
    ;call near timing
    ret
AfterRun:
      jmp $                      ;无限循环
count dw delay
count_t dw delay_t
Message db 'Hello, MyOs is loading user programs...'
MessageLength  equ ($-Message)
Message_S db 'Please select prgram you want to run...'
Message_S_length equ ($ - Message_S)
Message_1 db 'Loading program 1...'
Message_2 db 'Loading program 2...'
Message_3 db 'Loading program 3...'
Message_4 db 'Loading program 4...'
Message_Noti_length equ ($-Message_4)
      times 510-($-$$) db 0
      db 0x55,0xaa
