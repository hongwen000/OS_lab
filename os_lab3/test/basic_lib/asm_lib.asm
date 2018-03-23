BITS 16
[global sys_bios_clear_screen]
[global sys_bios_print_string]
[global sys_bios_getchar]

sys_bios_print_string:
    push bp
    mov bp, sp
    mov cx, word[bp + 10]    
    mov bx, word[bp + 14]    
    mov bh, 0
    mov dx, word[bp + 18]   
	mov	ax, ds		        
	mov	es, ax		        
	mov	ax, 1301h		    
    mov gs, bp
    mov bp, word[bp + 6]    
	int	10h			        
    mov bp, gs
    pop bp
    ret

sys_bios_clear_screen:
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

sys_bios_getchar:
    mov ah, 0
    int 16h
    ret
