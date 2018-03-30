;程序源代码（myos1.asm）
[BITS 16]
org  7c00h		; BIOS将把引导扇区加载到0:7C00h处，并开始执行

%define KERNEL_LOAD_ADDR 0x00007E00

%define KERNEL_ENTRY_ADDR 0x00007E00

%define KERNEL_SIZE      31

jmp bootloader_start

bootloader_start:

LoadnEx:
     ;读软盘或硬盘上的若干物理扇区到内存的ES:BX处：
      mov ax,cs                ;段地址 ; 存放数据的内存基地址
      mov es,ax                ;设置段地址（不能直接mov es,段地址）
      mov ds,ax                ;设置数据段
      mov bx, KERNEL_LOAD_ADDR ;内核加载地址
      mov ah,2                 ;功能号
      mov al,KERNEL_SIZE       ;扇区数
      mov dl,0                 ;驱动器号 ; 软盘为0，硬盘和U盘为80H
      mov dh,0                 ;磁头号 ; 起始编号为0
      mov ch,0                 ;柱面号 ; 起始编号为0
      mov cl,2                 ;起始扇区号 ; 起始编号为1
      int 13H                  ;调用读磁盘BIOS的13h功能
      jmp KERNEL_ENTRY_ADDR

      times 510-($-$$) db 0
      db 0x55,0xaa

