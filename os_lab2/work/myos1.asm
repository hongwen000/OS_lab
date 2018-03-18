;程序源代码（myos1.asm）
%macro print_string 4
	mov	ax, cs	       ; 置其他段寄存器值与CS相同
	mov	ds, ax	       ; 数据段
	mov	bp, %1		 ; BP=当前串的偏移地址
	mov	ax, ds		 ; ES:BP = 串地址
	mov	es, ax		 ; 置ES=DS
	mov	cx, %2             ; CX = 串长（=9）
	mov	ax, 1301h		 ; AH = 13h（功能号）、AL = 01h（光标置于串尾）
	mov	bx, 0007h		 ; 页号为0(BH = 0) 黑底白字(BL = 07h)
      mov   dh, %3		 ; 行号=0
	mov	dl, %4	       ; 列号=0
	int	10h			 ; BIOS的10h功能：显示一行字符
%endmacro

%macro install_int 2
      push es
      mov ax, 0
      mov es, ax
      mov word[es : %1*4], %2
      mov word[es : %1*4 +2], cs
      pop ax
      mov es, ax
%endmacro

%macro set_psp 1
      mov word[0xA000], 0xCD
      mov word[0xA000 + 2], 20h
      mov word[0xA00A], %1
      mov word[0xA00A + 2], cs
%endmacro

org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行

jmp Start

Message:          db 'Welcome to HHOS ver1.01', 0dh, 0ah
MessageLength     equ   ($-Message)
Promot:           db 'shell>'
PromotLength      equ ($-Promot)
Input:            db 0
OffSetOfUserPrg1  equ 0xA100
panic_21h_msg     dw 'Currently only 4ch function of int 21h is implemented'
paini_21h_len     equ ($-panic_21h_msg)

Start:
    call cls
    print_string Message, MessageLength, 0, 0
    print_string Promot, PromotLength, 1, 0
    install_int 20h, interrupt_20h
    install_int 21h, interrupt_21h
WaitInput:
      mov   ah, 0
      int   16h
      sub   al, 0x30
      mov   byte[Input], al
      ;call save_cursor
LoadnEx:
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
      mov ax,cs                ;段地址 ; 存放数据的内存基地址
      mov es,ax                ;设置段地址（不能直接mov es,段地址）
      mov bx, OffSetOfUserPrg1  ;偏移地址; 存放数据的内存偏移地址
      mov ah,2                 ; 功能号
      mov al,2                 ;扇区数
      mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
      mov dh,0                 ;磁头号 ; 起始编号为0
      mov ch,0                 ;柱面号 ; 起始编号为0
      mov cl,byte[Input]       ;起始扇区号 ; 起始编号为1
      add cl, cl               
      add cl, 1                ;user1->3, user2->5, ... userN->2*N + 1
      int 13H ;                调用读磁盘BIOS的13h功能
      ; 用户程序a.com已加载到指定内存区域中
      set_psp AfterRun
      pusha
      jmp OffSetOfUserPrg1
AfterRun:
      popa
    jmp Start

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


interrupt_20h:
      mov ah, 4ch
interrupt_21h:
      cmp ah, 4ch
      jnz panic_21h_func_not_impl
      jmp dword[0xA00A]
      iret
panic_21h_func_not_impl:
      print_string panic_21h_msg, paini_21h_len, 0, 0
      jmp $

      times 510-($-$$) db 0
      db 0x55,0xaa

