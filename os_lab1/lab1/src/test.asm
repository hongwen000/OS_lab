org 0x7c00
mov ax, 0xB800
mov gs, ax
xor ax,ax
mov ax,1
mov bx,80
mul bx
add ax,1
mov bx,2
mul bx
mov bp,ax
mov ah,1
mov al,41
mov word [gs:bp],ax
jmp $
times 510 - ($ -$$)     db  0
dw    0xaa55
