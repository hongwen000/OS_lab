	.file	"sys_io.cpp"
	.code16gcc
	.text
.Ltext0:
/APP
	.code16gcc

/NO_APP
	.globl	sys_bios_print_int
	.type	sys_bios_print_int, @function
sys_bios_print_int:
.LFB0:
	.file 1 "basic_lib/sys_io.cpp"
	.loc 1 3 0
	.cfi_startproc
.LVL0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	pushl	%eax
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
.LVL1:
	.loc 1 4 0
	movl	8(%ebp), %eax
	.loc 1 5 0
	movl	$1, %ecx
	.loc 1 6 0
	movl	$10, %ebx
.LVL2:
.L3:
	.loc 1 6 0 is_stmt 0 discriminator 2
	cltd
	idivl	%ebx
.LVL3:
	testl	%eax, %eax
	je	.L2
	.loc 1 6 0 discriminator 1
	incl	%ecx
.LVL4:
	jmp	.L3
.L2:
.LVL5:
	.loc 1 7 0 is_stmt 1
	leal	4(%ecx), %eax
.LVL6:
	andl	$-4, %eax
	subl	%eax, %esp
	movl	%esp, %edi
.LVL7:
	leal	(%esp,%ecx), %ebx
	.loc 1 8 0
	movl	$1, -16(%ebp)
.LVL8:
.L4:
.LBB8:
	.loc 1 10 0 discriminator 2
	movl	8(%ebp), %eax
	cltd
	idivl	-16(%ebp)
	movl	$10, %esi
	cltd
	idivl	%esi
	addl	$48, %edx
	movb	%dl, -1(%ebx)
.LVL9:
	.loc 1 9 0 discriminator 2
	imull	$10, -16(%ebp), %eax
	movl	%eax, -16(%ebp)
.LVL10:
	decl	%ebx
.LVL11:
	cmpl	%ebx, %edi
	jne	.L4
.LBE8:
	.loc 1 11 0
	movb	$0, (%edi,%ecx)
	.loc 1 12 0
	pushl	16(%ebp)
.LVL12:
	pushl	12(%ebp)
	pushl	%ecx
	pushl	%edi
	call	sys_bios_print_string
.LVL13:
	.loc 1 13 0
	addl	$16, %esp
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
.LVL14:
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE0:
	.size	sys_bios_print_int, .-sys_bios_print_int
	.globl	sys_putchar
	.type	sys_putchar, @function
sys_putchar:
.LFB3:
	.loc 1 69 0
	.cfi_startproc
.LVL15:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
.LVL16:
	.loc 1 71 0
	movl	12(%ebp), %eax
	sall	$8, %eax
	orl	8(%ebp), %eax
	.loc 1 70 0
	imull	$80, 16(%ebp), %ebx
	addl	20(%ebp), %ebx
	addl	%ebx, %ebx
	.loc 1 82 0
/APP
/  82 "basic_lib/sys_io.cpp" 1
	.intel_syntax noprefix
	pusha
	mov cx, 0xB800
	mov gs, cx
	mov bp,bx
	mov word ptr gs:[bp],ax
	popa
	.att_syntax noprefix
	
/  0 "" 2
	.loc 1 83 0
/NO_APP
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE3:
	.size	sys_putchar, .-sys_putchar
	.globl	sys_print_string
	.type	sys_print_string, @function
sys_print_string:
.LFB1:
	.loc 1 16 0
	.cfi_startproc
.LVL17:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%esi
	pushl	%ebx
	.cfi_offset 6, -12
	.cfi_offset 3, -16
	movl	20(%ebp), %ebx
.LVL18:
	movl	12(%ebp), %esi
	addl	%ebx, %esi
	.loc 1 16 0
	movl	%ebx, %edx
.LVL19:
.L12:
.LBB9:
	.loc 1 18 0 discriminator 3
	cmpl	%esi, %edx
	je	.L10
	.loc 1 20 0 discriminator 2
	pushl	%edx
	pushl	16(%ebp)
	pushl	$7
	movl	8(%ebp), %eax
	subl	%ebx, %eax
	movsbl	(%eax,%edx), %eax
	pushl	%eax
	call	sys_putchar
.LVL20:
	incl	%edx
.LVL21:
	.loc 1 18 0 discriminator 2
	addl	$16, %esp
	jmp	.L12
.L10:
.LBE9:
	.loc 1 22 0
	leal	-8(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
.LVL22:
	popl	%esi
	.cfi_restore 6
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE1:
	.size	sys_print_string, .-sys_print_string
	.globl	sys_print_int
	.type	sys_print_int, @function
sys_print_int:
.LFB2:
	.loc 1 25 0
	.cfi_startproc
.LVL23:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	pushl	%eax
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
.LVL24:
	.loc 1 26 0
	movl	8(%ebp), %eax
	.loc 1 27 0
	movl	$1, %ecx
	.loc 1 28 0
	movl	$10, %ebx
.LVL25:
.L16:
	.loc 1 28 0 is_stmt 0 discriminator 2
	cltd
	idivl	%ebx
.LVL26:
	testl	%eax, %eax
	je	.L15
	.loc 1 28 0 discriminator 1
	incl	%ecx
.LVL27:
	jmp	.L16
.L15:
.LVL28:
	.loc 1 29 0 is_stmt 1
	leal	4(%ecx), %eax
.LVL29:
	andl	$-4, %eax
	subl	%eax, %esp
	movl	%esp, %edi
.LVL30:
	leal	(%esp,%ecx), %ebx
	.loc 1 30 0
	movl	$1, -16(%ebp)
.LVL31:
.L17:
.LBB10:
	.loc 1 32 0 discriminator 2
	movl	8(%ebp), %eax
	cltd
	idivl	-16(%ebp)
	movl	$10, %esi
	cltd
	idivl	%esi
	addl	$48, %edx
	movb	%dl, -1(%ebx)
.LVL32:
	.loc 1 31 0 discriminator 2
	imull	$10, -16(%ebp), %eax
	movl	%eax, -16(%ebp)
.LVL33:
	decl	%ebx
.LVL34:
	cmpl	%ebx, %edi
	jne	.L17
.LBE10:
	.loc 1 33 0
	movb	$0, (%edi,%ecx)
	.loc 1 34 0
	pushl	16(%ebp)
.LVL35:
	pushl	12(%ebp)
	pushl	%ecx
	pushl	%edi
	call	sys_print_string
.LVL36:
	.loc 1 35 0
	addl	$16, %esp
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
.LVL37:
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE2:
	.size	sys_print_int, .-sys_print_int
	.globl	sys_bios_putchar
	.type	sys_bios_putchar, @function
sys_bios_putchar:
.LFB4:
	.loc 1 86 0
	.cfi_startproc
.LVL38:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%eax
	movl	8(%ebp), %eax
	movb	%al, -4(%ebp)
	.loc 1 87 0
	movl	16(%ebp), %eax
	sall	$8, %eax
	movzbl	20(%ebp), %edx
	orl	%edx, %eax
	pushl	%eax
	pushl	12(%ebp)
	pushl	$1
	leal	-4(%ebp), %eax
	pushl	%eax
	call	sys_bios_print_string
.LVL39:
	.loc 1 88 0
	addl	$16, %esp
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE4:
	.size	sys_bios_putchar, .-sys_bios_putchar
	.globl	sys_inb
	.type	sys_inb, @function
sys_inb:
.LFB5:
	.loc 1 91 0
	.cfi_startproc
.LVL40:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	.loc 1 95 0
	movl	8(%ebp), %edx
/APP
/  95 "basic_lib/sys_io.cpp" 1
	inb %dx, %al
/  0 "" 2
.LVL41:
	.loc 1 97 0
/NO_APP
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE5:
	.size	sys_inb, .-sys_inb
	.globl	sys_outb
	.type	sys_outb, @function
sys_outb:
.LFB6:
	.loc 1 111 0
	.cfi_startproc
.LVL42:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	.loc 1 114 0
	movl	8(%ebp), %edx
	movl	12(%ebp), %eax
/APP
/  114 "basic_lib/sys_io.cpp" 1
	outb %al, %dx
/  0 "" 2
	.loc 1 115 0
/NO_APP
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE6:
	.size	sys_outb, .-sys_outb
	.globl	sys_dbg_bochs_putc
	.type	sys_dbg_bochs_putc, @function
sys_dbg_bochs_putc:
.LFB7:
	.loc 1 125 0
	.cfi_startproc
.LVL43:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
.LVL44:
.LBB11:
.LBB12:
	.loc 1 114 0
	movl	8(%ebp), %eax
/APP
/  114 "basic_lib/sys_io.cpp" 1
	outb %al, $233
/  0 "" 2
.LVL45:
/NO_APP
.LBE12:
.LBE11:
	.loc 1 127 0
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE7:
	.size	sys_dbg_bochs_putc, .-sys_dbg_bochs_putc
	.globl	sys_get_scancode
	.type	sys_get_scancode, @function
sys_get_scancode:
.LFB8:
	.loc 1 130 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
.L29:
.LVL46:
.LBB13:
.LBB14:
	.loc 1 95 0
/APP
/  95 "basic_lib/sys_io.cpp" 1
	inb $100, %al
/  0 "" 2
.LVL47:
/NO_APP
.LBE14:
.LBE13:
	.loc 1 131 0
	testb	$1, %al
	je	.L29
.LVL48:
.LBB15:
.LBB16:
	.loc 1 95 0
/APP
/  95 "basic_lib/sys_io.cpp" 1
	inb $96, %al
/  0 "" 2
.LVL49:
/NO_APP
.LBE16:
.LBE15:
	.loc 1 133 0
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE8:
	.size	sys_get_scancode, .-sys_get_scancode
	.globl	sys_read_disk
	.type	sys_read_disk, @function
sys_read_disk:
.LFB9:
	.loc 1 136 0
	.cfi_startproc
.LVL50:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	12(%ebp), %eax
	.loc 1 137 0
	movl	$36, %ecx
	xorl	%edx, %edx
	divl	%ecx
	movl	%eax, %ebx
.LVL51:
	.loc 1 138 0
	movl	$18, %ecx
	movl	%edx, %eax
.LVL52:
	xorl	%edx, %edx
.LVL53:
	divl	%ecx
.LVL54:
	.loc 1 140 0
	movl	%ebx, %ecx
	shrl	$2, %ecx
.LVL55:
	andw	$192, %cx
.LVL56:
	.loc 1 139 0
	incl	%edx
.LVL57:
	.loc 1 140 0
	orl	%edx, %ecx
	sall	$8, %ebx
.LVL58:
	orl	%ebx, %ecx
	.loc 1 149 0
/APP
/  149 "basic_lib/sys_io.cpp" 1
	movw 8(%ebp), %bx
	movb %eax, %dh
	movw %cx, %cx
	movb 16(%ebp), %al
	movb $0, %dl
	movb $2, %ah
	int $0x13
	
/  0 "" 2
	.loc 1 150 0
/NO_APP
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE9:
	.size	sys_read_disk, .-sys_read_disk
.Letext0:
	.file 2 "/opt/cross/gcc-i386/lib/gcc/i386-elf/7.3.0/include/stddef.h"
	.file 3 "/opt/cross/gcc-i386/lib/gcc/i386-elf/7.3.0/include/stdint-gcc.h"
	.file 4 "basic_lib/../include/defines.h"
	.file 5 "basic_lib/sys_lib.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x59a
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF51
	.byte	0x4
	.long	.LASF52
	.long	.LASF53
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF0
	.uleb128 0x2
	.byte	0x4
	.byte	0x5
	.long	.LASF1
	.uleb128 0x3
	.long	.LASF8
	.byte	0x2
	.byte	0xd8
	.long	0x3e
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF3
	.uleb128 0x2
	.byte	0xc
	.byte	0x4
	.long	.LASF4
	.uleb128 0x2
	.byte	0x10
	.byte	0x4
	.long	.LASF5
	.uleb128 0x4
	.long	.LASF54
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF6
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF7
	.uleb128 0x3
	.long	.LASF9
	.byte	0x3
	.byte	0x2e
	.long	0x78
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF10
	.uleb128 0x3
	.long	.LASF11
	.byte	0x3
	.byte	0x31
	.long	0x8a
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF12
	.uleb128 0x3
	.long	.LASF13
	.byte	0x3
	.byte	0x34
	.long	0x3e
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF14
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF15
	.uleb128 0x6
	.long	0xaa
	.uleb128 0x7
	.long	.LASF55
	.byte	0x7
	.byte	0x4
	.long	0x25
	.byte	0x4
	.byte	0x17
	.long	0x128
	.uleb128 0x8
	.long	.LASF16
	.byte	0
	.uleb128 0x8
	.long	.LASF17
	.byte	0x1
	.uleb128 0x8
	.long	.LASF18
	.byte	0x2
	.uleb128 0x8
	.long	.LASF19
	.byte	0x3
	.uleb128 0x8
	.long	.LASF20
	.byte	0x4
	.uleb128 0x8
	.long	.LASF21
	.byte	0x5
	.uleb128 0x8
	.long	.LASF22
	.byte	0x6
	.uleb128 0x8
	.long	.LASF23
	.byte	0x7
	.uleb128 0x8
	.long	.LASF24
	.byte	0x8
	.uleb128 0x8
	.long	.LASF25
	.byte	0x9
	.uleb128 0x8
	.long	.LASF26
	.byte	0xa
	.uleb128 0x8
	.long	.LASF27
	.byte	0xb
	.uleb128 0x8
	.long	.LASF28
	.byte	0xc
	.uleb128 0x8
	.long	.LASF29
	.byte	0xd
	.uleb128 0x8
	.long	.LASF30
	.byte	0xe
	.uleb128 0x8
	.long	.LASF31
	.byte	0xf
	.byte	0
	.uleb128 0x9
	.long	.LASF38
	.byte	0x1
	.byte	0x87
	.long	.LFB9
	.long	.LFE9-.LFB9
	.uleb128 0x1
	.byte	0x9c
	.long	0x1a4
	.uleb128 0xa
	.long	.LASF32
	.byte	0x1
	.byte	0x87
	.long	0x91
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xa
	.long	.LASF33
	.byte	0x1
	.byte	0x87
	.long	0x33
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0xa
	.long	.LASF34
	.byte	0x1
	.byte	0x87
	.long	0x33
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0xb
	.long	.LASF35
	.byte	0x1
	.byte	0x89
	.long	0x33
	.long	.LLST24
	.uleb128 0xb
	.long	.LASF36
	.byte	0x1
	.byte	0x8a
	.long	0x33
	.long	.LLST25
	.uleb128 0xb
	.long	.LASF37
	.byte	0x1
	.byte	0x8b
	.long	0x33
	.long	.LLST26
	.uleb128 0xc
	.string	"_cx"
	.byte	0x1
	.byte	0x8c
	.long	0x7f
	.long	.LLST27
	.byte	0
	.uleb128 0xd
	.long	.LASF56
	.byte	0x1
	.byte	0x81
	.long	0xaa
	.long	.LFB8
	.long	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0x212
	.uleb128 0xe
	.long	0x279
	.long	.LBB13
	.long	.LBE13-.LBB13
	.byte	0x1
	.byte	0x83
	.long	0x1e9
	.uleb128 0xf
	.long	0x289
	.long	.LLST22
	.uleb128 0x10
	.long	.LBB14
	.long	.LBE14-.LBB14
	.uleb128 0x11
	.long	0x566
	.byte	0
	.byte	0
	.uleb128 0x12
	.long	0x279
	.long	.LBB15
	.long	.LBE15-.LBB15
	.byte	0x1
	.byte	0x84
	.uleb128 0xf
	.long	0x289
	.long	.LLST23
	.uleb128 0x10
	.long	.LBB16
	.long	.LBE16-.LBB16
	.uleb128 0x11
	.long	0x566
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x9
	.long	.LASF39
	.byte	0x1
	.byte	0x7d
	.long	.LFB7
	.long	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.long	0x256
	.uleb128 0x13
	.string	"c"
	.byte	0x1
	.byte	0x7d
	.long	0xaa
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x12
	.long	0x256
	.long	.LBB11
	.long	.LBE11-.LBB11
	.byte	0x1
	.byte	0x7e
	.uleb128 0xf
	.long	0x26d
	.long	.LLST20
	.uleb128 0xf
	.long	0x262
	.long	.LLST21
	.byte	0
	.byte	0
	.uleb128 0x14
	.long	.LASF57
	.byte	0x1
	.byte	0x6e
	.byte	0x1
	.long	0x279
	.uleb128 0x15
	.long	.LASF40
	.byte	0x1
	.byte	0x6e
	.long	0x7f
	.uleb128 0x15
	.long	.LASF41
	.byte	0x1
	.byte	0x6e
	.long	0x6d
	.byte	0
	.uleb128 0x16
	.long	.LASF58
	.byte	0x1
	.byte	0x5a
	.long	0x6d
	.byte	0x1
	.long	0x2a0
	.uleb128 0x15
	.long	.LASF40
	.byte	0x1
	.byte	0x5a
	.long	0x7f
	.uleb128 0x17
	.string	"ret"
	.byte	0x1
	.byte	0x5c
	.long	0x6d
	.byte	0
	.uleb128 0x9
	.long	.LASF42
	.byte	0x1
	.byte	0x55
	.long	.LFB4
	.long	.LFE4-.LFB4
	.uleb128 0x1
	.byte	0x9c
	.long	0x2f2
	.uleb128 0x18
	.string	"c"
	.byte	0x1
	.byte	0x55
	.long	0xaa
	.long	.LLST19
	.uleb128 0xa
	.long	.LASF43
	.byte	0x1
	.byte	0x55
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x13
	.string	"x"
	.byte	0x1
	.byte	0x55
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x13
	.string	"y"
	.byte	0x1
	.byte	0x55
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 12
	.uleb128 0x19
	.long	.LVL39
	.long	0x592
	.byte	0
	.uleb128 0x9
	.long	.LASF44
	.byte	0x1
	.byte	0x44
	.long	.LFB3
	.long	.LFE3-.LFB3
	.uleb128 0x1
	.byte	0x9c
	.long	0x369
	.uleb128 0x13
	.string	"c"
	.byte	0x1
	.byte	0x44
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0xa
	.long	.LASF43
	.byte	0x1
	.byte	0x44
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x13
	.string	"x"
	.byte	0x1
	.byte	0x44
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x13
	.string	"y"
	.byte	0x1
	.byte	0x44
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 12
	.uleb128 0x1a
	.long	.LASF45
	.byte	0x1
	.byte	0x46
	.long	0xa3
	.uleb128 0xd
	.byte	0x91
	.sleb128 8
	.byte	0x6
	.byte	0x8
	.byte	0x50
	.byte	0x1e
	.byte	0x91
	.sleb128 12
	.byte	0x6
	.byte	0x22
	.byte	0x31
	.byte	0x24
	.byte	0x9f
	.uleb128 0x1b
	.string	"ch"
	.byte	0x1
	.byte	0x47
	.long	0x7f
	.uleb128 0xb
	.byte	0x91
	.sleb128 4
	.byte	0x6
	.byte	0x38
	.byte	0x24
	.byte	0x91
	.sleb128 0
	.byte	0x94
	.byte	0x2
	.byte	0x21
	.byte	0x9f
	.byte	0
	.uleb128 0x9
	.long	.LASF46
	.byte	0x1
	.byte	0x18
	.long	.LFB2
	.long	.LFE2-.LFB2
	.uleb128 0x1
	.byte	0x9c
	.long	0x408
	.uleb128 0x18
	.string	"num"
	.byte	0x1
	.byte	0x18
	.long	0xa3
	.long	.LLST11
	.uleb128 0x18
	.string	"x"
	.byte	0x1
	.byte	0x18
	.long	0xa3
	.long	.LLST12
	.uleb128 0x18
	.string	"y"
	.byte	0x1
	.byte	0x18
	.long	0xa3
	.long	.LLST13
	.uleb128 0xb
	.long	.LASF47
	.byte	0x1
	.byte	0x1a
	.long	0xa3
	.long	.LLST14
	.uleb128 0xc
	.string	"len"
	.byte	0x1
	.byte	0x1b
	.long	0xa3
	.long	.LLST15
	.uleb128 0xc
	.string	"arr"
	.byte	0x1
	.byte	0x1d
	.long	0x408
	.long	.LLST16
	.uleb128 0xb
	.long	.LASF48
	.byte	0x1
	.byte	0x1e
	.long	0xa3
	.long	.LLST17
	.uleb128 0x1c
	.long	.LBB10
	.long	.LBE10-.LBB10
	.long	0x3fe
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x1f
	.long	0xa3
	.long	.LLST18
	.byte	0
	.uleb128 0x19
	.long	.LVL36
	.long	0x41a
	.byte	0
	.uleb128 0x1d
	.long	0xaa
	.long	0x41a
	.uleb128 0x1e
	.long	0x3e
	.uleb128 0x2
	.byte	0x71
	.sleb128 0
	.byte	0
	.uleb128 0x9
	.long	.LASF49
	.byte	0x1
	.byte	0xf
	.long	.LFB1
	.long	.LFE1-.LFB1
	.uleb128 0x1
	.byte	0x9c
	.long	0x490
	.uleb128 0x13
	.string	"str"
	.byte	0x1
	.byte	0xf
	.long	0x490
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x13
	.string	"len"
	.byte	0x1
	.byte	0xf
	.long	0x25
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.uleb128 0x13
	.string	"x"
	.byte	0x1
	.byte	0xf
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x13
	.string	"y"
	.byte	0x1
	.byte	0xf
	.long	0xa3
	.uleb128 0x2
	.byte	0x91
	.sleb128 12
	.uleb128 0x1f
	.long	.LASF43
	.byte	0x1
	.byte	0x11
	.long	0xa3
	.byte	0x7
	.uleb128 0x10
	.long	.LBB9
	.long	.LBE9-.LBB9
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x12
	.long	0x25
	.long	.LLST9
	.uleb128 0x19
	.long	.LVL20
	.long	0x2f2
	.byte	0
	.byte	0
	.uleb128 0x20
	.byte	0x4
	.long	0xb1
	.uleb128 0x9
	.long	.LASF50
	.byte	0x1
	.byte	0x2
	.long	.LFB0
	.long	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.long	0x539
	.uleb128 0x18
	.string	"num"
	.byte	0x1
	.byte	0x2
	.long	0xa3
	.long	.LLST1
	.uleb128 0x21
	.long	.LASF43
	.byte	0x1
	.byte	0x2
	.long	0xa3
	.long	.LLST2
	.uleb128 0x18
	.string	"pos"
	.byte	0x1
	.byte	0x2
	.long	0xa3
	.long	.LLST3
	.uleb128 0xb
	.long	.LASF47
	.byte	0x1
	.byte	0x4
	.long	0xa3
	.long	.LLST4
	.uleb128 0xc
	.string	"len"
	.byte	0x1
	.byte	0x5
	.long	0xa3
	.long	.LLST5
	.uleb128 0xc
	.string	"arr"
	.byte	0x1
	.byte	0x7
	.long	0x539
	.long	.LLST6
	.uleb128 0xb
	.long	.LASF48
	.byte	0x1
	.byte	0x8
	.long	0xa3
	.long	.LLST7
	.uleb128 0x1c
	.long	.LBB8
	.long	.LBE8-.LBB8
	.long	0x52f
	.uleb128 0xc
	.string	"i"
	.byte	0x1
	.byte	0x9
	.long	0xa3
	.long	.LLST8
	.byte	0
	.uleb128 0x19
	.long	.LVL13
	.long	0x592
	.byte	0
	.uleb128 0x1d
	.long	0xaa
	.long	0x54b
	.uleb128 0x1e
	.long	0x3e
	.uleb128 0x2
	.byte	0x71
	.sleb128 0
	.byte	0
	.uleb128 0x22
	.long	0x279
	.long	.LFB5
	.long	.LFE5-.LFB5
	.uleb128 0x1
	.byte	0x9c
	.long	0x56e
	.uleb128 0x23
	.long	0x289
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x24
	.long	0x294
	.uleb128 0x1
	.byte	0x50
	.byte	0
	.uleb128 0x22
	.long	0x256
	.long	.LFB6
	.long	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.long	0x592
	.uleb128 0x23
	.long	0x262
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x23
	.long	0x26d
	.uleb128 0x2
	.byte	0x91
	.sleb128 4
	.byte	0
	.uleb128 0x25
	.long	.LASF59
	.long	.LASF59
	.byte	0x5
	.byte	0xb
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1b
	.uleb128 0xe
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x3b
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x4
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x1d
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x58
	.uleb128 0xb
	.uleb128 0x59
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x20
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1e
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1f
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x20
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x21
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x22
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x6
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2117
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x23
	.uleb128 0x5
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x24
	.uleb128 0x34
	.byte	0
	.uleb128 0x31
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x25
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x6e
	.uleb128 0xe
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_loc,"",@progbits
.Ldebug_loc0:
.LLST24:
	.long	.LVL51-.Ltext0
	.long	.LVL52-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL52-.Ltext0
	.long	.LVL58-.Ltext0
	.value	0x1
	.byte	0x53
	.long	.LVL58-.Ltext0
	.long	.LFE9-.Ltext0
	.value	0xd
	.byte	0x91
	.sleb128 4
	.byte	0x6
	.byte	0xf7
	.uleb128 0x25
	.byte	0x8
	.byte	0x24
	.byte	0xf7
	.uleb128 0x25
	.byte	0x1b
	.byte	0xf7
	.uleb128 0
	.byte	0x9f
	.long	0
	.long	0
.LLST25:
	.long	.LVL51-.Ltext0
	.long	.LVL53-.Ltext0
	.value	0xb
	.byte	0x72
	.sleb128 0
	.byte	0xf7
	.uleb128 0x25
	.byte	0x42
	.byte	0xf7
	.uleb128 0x25
	.byte	0x1b
	.byte	0xf7
	.uleb128 0
	.byte	0x9f
	.long	.LVL53-.Ltext0
	.long	.LVL54-.Ltext0
	.value	0xb
	.byte	0x70
	.sleb128 0
	.byte	0xf7
	.uleb128 0x25
	.byte	0x42
	.byte	0xf7
	.uleb128 0x25
	.byte	0x1b
	.byte	0xf7
	.uleb128 0
	.byte	0x9f
	.long	.LVL54-.Ltext0
	.long	.LFE9-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0
	.long	0
.LLST26:
	.long	.LVL51-.Ltext0
	.long	.LVL53-.Ltext0
	.value	0x7
	.byte	0x72
	.sleb128 0
	.byte	0x42
	.byte	0x1d
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.long	.LVL53-.Ltext0
	.long	.LVL54-.Ltext0
	.value	0x7
	.byte	0x70
	.sleb128 0
	.byte	0x42
	.byte	0x1d
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.long	.LVL54-.Ltext0
	.long	.LVL57-.Ltext0
	.value	0xb
	.byte	0x91
	.sleb128 4
	.byte	0x6
	.byte	0x8
	.byte	0x24
	.byte	0x1d
	.byte	0x42
	.byte	0x1d
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.long	.LVL57-.Ltext0
	.long	.LFE9-.Ltext0
	.value	0x1
	.byte	0x52
	.long	0
	.long	0
.LLST27:
	.long	.LVL51-.Ltext0
	.long	.LVL52-.Ltext0
	.value	0x14
	.byte	0x70
	.sleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x72
	.sleb128 0
	.byte	0x42
	.byte	0x1d
	.byte	0x23
	.uleb128 0x1
	.byte	0x21
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x9f
	.long	.LVL52-.Ltext0
	.long	.LVL53-.Ltext0
	.value	0x14
	.byte	0x73
	.sleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x72
	.sleb128 0
	.byte	0x42
	.byte	0x1d
	.byte	0x23
	.uleb128 0x1
	.byte	0x21
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x9f
	.long	.LVL53-.Ltext0
	.long	.LVL54-.Ltext0
	.value	0x14
	.byte	0x73
	.sleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x70
	.sleb128 0
	.byte	0x42
	.byte	0x1d
	.byte	0x23
	.uleb128 0x1
	.byte	0x21
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x9f
	.long	.LVL54-.Ltext0
	.long	.LVL55-.Ltext0
	.value	0x10
	.byte	0x73
	.sleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x72
	.sleb128 1
	.byte	0x21
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x9f
	.long	.LVL55-.Ltext0
	.long	.LVL56-.Ltext0
	.value	0xe
	.byte	0x71
	.sleb128 0
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x72
	.sleb128 1
	.byte	0x21
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x9f
	.long	.LVL56-.Ltext0
	.long	.LVL57-.Ltext0
	.value	0x10
	.byte	0x73
	.sleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x72
	.sleb128 1
	.byte	0x21
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x9f
	.long	.LVL57-.Ltext0
	.long	.LVL58-.Ltext0
	.value	0x10
	.byte	0x73
	.sleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x73
	.sleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x72
	.sleb128 0
	.byte	0x21
	.byte	0x9f
	.long	.LVL58-.Ltext0
	.long	.LFE9-.Ltext0
	.value	0x24
	.byte	0x91
	.sleb128 4
	.byte	0x6
	.byte	0xf7
	.uleb128 0x25
	.byte	0x8
	.byte	0x24
	.byte	0xf7
	.uleb128 0x25
	.byte	0x1b
	.byte	0xf7
	.uleb128 0
	.byte	0x32
	.byte	0x25
	.byte	0x8
	.byte	0xc0
	.byte	0x1a
	.byte	0x91
	.sleb128 4
	.byte	0x6
	.byte	0xf7
	.uleb128 0x25
	.byte	0x8
	.byte	0x24
	.byte	0xf7
	.uleb128 0x25
	.byte	0x1b
	.byte	0xf7
	.uleb128 0
	.byte	0x38
	.byte	0x24
	.byte	0x21
	.byte	0x72
	.sleb128 0
	.byte	0x21
	.byte	0x9f
	.long	0
	.long	0
.LLST22:
	.long	.LVL46-.Ltext0
	.long	.LVL47-.Ltext0
	.value	0x3
	.byte	0x8
	.byte	0x64
	.byte	0x9f
	.long	0
	.long	0
.LLST23:
	.long	.LVL48-.Ltext0
	.long	.LVL49-.Ltext0
	.value	0x3
	.byte	0x8
	.byte	0x60
	.byte	0x9f
	.long	0
	.long	0
.LLST20:
	.long	.LVL44-.Ltext0
	.long	.LVL45-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	0
	.long	0
.LLST21:
	.long	.LVL44-.Ltext0
	.long	.LVL45-.Ltext0
	.value	0x3
	.byte	0x8
	.byte	0xe9
	.byte	0x9f
	.long	0
	.long	0
.LLST19:
	.long	.LVL38-.Ltext0
	.long	.LVL39-1-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	0
	.long	0
.LLST11:
	.long	.LVL23-.Ltext0
	.long	.LVL35-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	0
	.long	0
.LLST12:
	.long	.LVL23-.Ltext0
	.long	.LVL35-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 4
	.long	0
	.long	0
.LLST13:
	.long	.LVL23-.Ltext0
	.long	.LVL35-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 8
	.long	0
	.long	0
.LLST14:
	.long	.LVL24-.Ltext0
	.long	.LVL25-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL25-.Ltext0
	.long	.LVL29-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0
	.long	0
.LLST15:
	.long	.LVL24-.Ltext0
	.long	.LVL25-.Ltext0
	.value	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL25-.Ltext0
	.long	.LVL36-1-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0
	.long	0
.LLST16:
	.long	.LVL30-.Ltext0
	.long	.LVL37-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 0
	.long	0
	.long	0
.LLST17:
	.long	.LVL30-.Ltext0
	.long	.LVL31-.Ltext0
	.value	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL31-.Ltext0
	.long	.LVL33-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -24
	.long	.LVL33-.Ltext0
	.long	.LVL36-1-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL36-1-.Ltext0
	.long	.LFE2-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -24
	.long	0
	.long	0
.LLST18:
	.long	.LVL30-.Ltext0
	.long	.LVL31-.Ltext0
	.value	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL31-.Ltext0
	.long	.LVL32-.Ltext0
	.value	0x9
	.byte	0x77
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL32-.Ltext0
	.long	.LVL34-.Ltext0
	.value	0xb
	.byte	0x77
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.long	.LVL34-.Ltext0
	.long	.LVL36-1-.Ltext0
	.value	0x9
	.byte	0x77
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST9:
	.long	.LVL18-.Ltext0
	.long	.LVL19-.Ltext0
	.value	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL19-.Ltext0
	.long	.LVL20-.Ltext0
	.value	0x6
	.byte	0x72
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL20-.Ltext0
	.long	.LVL21-.Ltext0
	.value	0x8
	.byte	0x72
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.long	.LVL21-.Ltext0
	.long	.LVL22-.Ltext0
	.value	0x6
	.byte	0x72
	.sleb128 0
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL22-.Ltext0
	.long	.LFE1-.Ltext0
	.value	0x7
	.byte	0x72
	.sleb128 0
	.byte	0x91
	.sleb128 12
	.byte	0x6
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
.LLST1:
	.long	.LVL0-.Ltext0
	.long	.LVL12-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	0
	.long	0
.LLST2:
	.long	.LVL0-.Ltext0
	.long	.LVL12-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 4
	.long	0
	.long	0
.LLST3:
	.long	.LVL0-.Ltext0
	.long	.LVL12-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 8
	.long	0
	.long	0
.LLST4:
	.long	.LVL1-.Ltext0
	.long	.LVL2-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL2-.Ltext0
	.long	.LVL6-.Ltext0
	.value	0x1
	.byte	0x50
	.long	0
	.long	0
.LLST5:
	.long	.LVL1-.Ltext0
	.long	.LVL2-.Ltext0
	.value	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL2-.Ltext0
	.long	.LVL13-1-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0
	.long	0
.LLST6:
	.long	.LVL7-.Ltext0
	.long	.LVL14-.Ltext0
	.value	0x2
	.byte	0x77
	.sleb128 0
	.long	0
	.long	0
.LLST7:
	.long	.LVL7-.Ltext0
	.long	.LVL8-.Ltext0
	.value	0x2
	.byte	0x31
	.byte	0x9f
	.long	.LVL8-.Ltext0
	.long	.LVL10-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -24
	.long	.LVL10-.Ltext0
	.long	.LVL13-1-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL13-1-.Ltext0
	.long	.LFE0-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -24
	.long	0
	.long	0
.LLST8:
	.long	.LVL7-.Ltext0
	.long	.LVL8-.Ltext0
	.value	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL8-.Ltext0
	.long	.LVL9-.Ltext0
	.value	0x9
	.byte	0x77
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	.LVL9-.Ltext0
	.long	.LVL11-.Ltext0
	.value	0xb
	.byte	0x77
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x23
	.uleb128 0x1
	.byte	0x9f
	.long	.LVL11-.Ltext0
	.long	.LVL13-1-.Ltext0
	.value	0x9
	.byte	0x77
	.sleb128 0
	.byte	0x71
	.sleb128 0
	.byte	0x22
	.byte	0x73
	.sleb128 0
	.byte	0x1c
	.byte	0x9f
	.long	0
	.long	0
	.section	.debug_aranges,"",@progbits
	.long	0x1c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x4
	.byte	0
	.value	0
	.value	0
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	0
	.long	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF22:
	.string	"VGA_BROWN"
.LASF54:
	.string	"decltype(nullptr)"
.LASF39:
	.string	"sys_dbg_bochs_putc"
.LASF47:
	.string	"num_"
.LASF59:
	.string	"sys_bios_print_string"
.LASF7:
	.string	"short int"
.LASF8:
	.string	"size_t"
.LASF26:
	.string	"VGA_BRIGHT_GREEN"
.LASF43:
	.string	"color"
.LASF21:
	.string	"VGA_MAGENTA"
.LASF18:
	.string	"VGA_GREEN"
.LASF30:
	.string	"VGA_YELLOW"
.LASF25:
	.string	"VGA_BRIGHT_BLUE"
.LASF51:
	.string	"GNU C++14 7.3.0 -m16 -march=i386 -mpreferred-stack-boundary=2 -g -Os -std=c++1z -ffreestanding -fno-builtin -fno-omit-frame-pointer -fno-tree-ch -fno-stack-protector"
.LASF36:
	.string	"head"
.LASF9:
	.string	"uint8_t"
.LASF56:
	.string	"sys_get_scancode"
.LASF24:
	.string	"VGA_GRAY"
.LASF53:
	.string	"/Users/lixinrui/onedrive/Documents/\345\244\247\345\255\246\350\257\276\347\250\213/2018\346\223\215\344\275\234\347\263\273\347\273\237\345\256\236\351\252\214/os_lab3/test"
.LASF37:
	.string	"sector"
.LASF35:
	.string	"cylinder"
.LASF33:
	.string	"logical_start_sector"
.LASF3:
	.string	"long long int"
.LASF48:
	.string	"power"
.LASF19:
	.string	"VGA_CYAN"
.LASF1:
	.string	"long int"
.LASF32:
	.string	"address"
.LASF45:
	.string	"offset"
.LASF50:
	.string	"sys_bios_print_int"
.LASF34:
	.string	"secotr_cnt"
.LASF4:
	.string	"long double"
.LASF10:
	.string	"unsigned char"
.LASF6:
	.string	"signed char"
.LASF14:
	.string	"long long unsigned int"
.LASF13:
	.string	"uint32_t"
.LASF0:
	.string	"unsigned int"
.LASF11:
	.string	"uint16_t"
.LASF49:
	.string	"sys_print_string"
.LASF27:
	.string	"VGA_BRIGHT_CYAN"
.LASF42:
	.string	"sys_bios_putchar"
.LASF12:
	.string	"short unsigned int"
.LASF31:
	.string	"VGA_BRIGHT_WHITE"
.LASF15:
	.string	"char"
.LASF23:
	.string	"VGA_WHITE"
.LASF5:
	.string	"__float128"
.LASF41:
	.string	"data"
.LASF16:
	.string	"VGA_BLACK"
.LASF40:
	.string	"port"
.LASF2:
	.string	"long unsigned int"
.LASF20:
	.string	"VGA_RED"
.LASF55:
	.string	"VGA_COLOR"
.LASF58:
	.string	"sys_inb"
.LASF52:
	.string	"basic_lib/sys_io.cpp"
.LASF44:
	.string	"sys_putchar"
.LASF46:
	.string	"sys_print_int"
.LASF17:
	.string	"VGA_BLUE"
.LASF28:
	.string	"VGA_BRIGHT_RED"
.LASF38:
	.string	"sys_read_disk"
.LASF57:
	.string	"sys_outb"
.LASF29:
	.string	"VGA_BRIGHT_MAGENTA"
	.ident	"GCC: (GNU) 7.3.0"
