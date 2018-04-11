	.file	"cmos.cpp"
	.code16gcc
	.text
.Ltext0:
/APP
	.code16gcc

/NO_APP
	.globl	_Z9read_cmosPh
	.type	_Z9read_cmosPh, @function
_Z9read_cmosPh:
.LFB18:
	.file 1 "basic_lib/cmos.cpp"
	.loc 1 4 0
	.cfi_startproc
.LVL0:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%ebx
	.cfi_offset 3, -12
	movl	8(%ebp), %ecx
.LVL1:
	.loc 1 4 0
	xorl	%edx, %edx
.LVL2:
.L3:
	.loc 1 7 0 discriminator 3
	cmpl	$128, %edx
	je	.L1
	.loc 1 19 0 discriminator 2
/APP
/  19 "basic_lib/cmos.cpp" 1
	cli
	movb %dl, %al
	out %al, $0x70
	in $0x71, %al	sti
	movb %al, %bl
	
/  0 "" 2
.LVL3:
	.loc 1 20 0 discriminator 2
/NO_APP
	movb	%bl, (%ecx,%edx)
.LVL4:
	incl	%edx
.LVL5:
	.loc 1 7 0 discriminator 2
	jmp	.L3
.LVL6:
.L1:
	.loc 1 22 0
	popl	%ebx
	.cfi_restore 3
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE18:
	.size	_Z9read_cmosPh, .-_Z9read_cmosPh
	.globl	_Z27get_update_in_progress_flagv
	.type	_Z27get_update_in_progress_flagv, @function
_Z27get_update_in_progress_flagv:
.LFB19:
	.loc 1 39 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	.loc 1 40 0
	pushl	$10
	pushl	$112
	call	sys_outb
.LVL7:
	.loc 1 41 0
	popl	%eax
	popl	%edx
	pushl	$113
	call	sys_inb
.LVL8:
	andl	$-128, %eax
	movzbl	%al, %eax
	.loc 1 42 0
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	ret
	.cfi_endproc
.LFE19:
	.size	_Z27get_update_in_progress_flagv, .-_Z27get_update_in_progress_flagv
	.globl	_Z16get_RTC_registeri
	.type	_Z16get_RTC_registeri, @function
_Z16get_RTC_registeri:
.LFB20:
	.loc 1 44 0
	.cfi_startproc
.LVL9:
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	.loc 1 45 0
	movzbl	8(%ebp), %eax
	pushl	%eax
	pushl	$112
	call	sys_outb
.LVL10:
	.loc 1 46 0
	popl	%eax
	popl	%edx
	movl	$113, 8(%ebp)
.LVL11:
	.loc 1 47 0
	leave
	.cfi_restore 5
	.cfi_def_cfa 4, 4
	.loc 1 46 0
	jmp	sys_inb
.LVL12:
	.cfi_endproc
.LFE20:
	.size	_Z16get_RTC_registeri, .-_Z16get_RTC_registeri
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"%d:%d:%d\n"
	.text
	.globl	read_rtc
	.type	read_rtc, @function
read_rtc:
.LFB21:
	.loc 1 49 0
	.cfi_startproc
	pushl	%ebp
	.cfi_def_cfa_offset 8
	.cfi_offset 5, -8
	movl	%esp, %ebp
	.cfi_def_cfa_register 5
	pushl	%edi
	pushl	%esi
	pushl	%ebx
	.cfi_offset 7, -12
	.cfi_offset 6, -16
	.cfi_offset 3, -20
	pushl	%ebx
.L11:
	.loc 1 58 0
	call	_Z27get_update_in_progress_flagv
.LVL13:
	testl	%eax, %eax
	jne	.L11
	.loc 1 59 0
	pushl	$0
	call	_Z16get_RTC_registeri
.LVL14:
	movb	%al, sys_internal_second
	.loc 1 60 0
	movl	$2, (%esp)
	call	_Z16get_RTC_registeri
.LVL15:
	movb	%al, sys_internal_minute
	.loc 1 61 0
	movl	$4, (%esp)
	call	_Z16get_RTC_registeri
.LVL16:
	movb	%al, hour
	.loc 1 62 0
	movl	$7, (%esp)
	call	_Z16get_RTC_registeri
.LVL17:
	movb	%al, day
	.loc 1 63 0
	movl	$8, (%esp)
	call	_Z16get_RTC_registeri
.LVL18:
	movb	%al, month
	.loc 1 64 0
	movl	$9, (%esp)
	call	_Z16get_RTC_registeri
.LVL19:
	movzbl	%al, %eax
	movl	%eax, year
	popl	%ecx
.L20:
	.loc 1 67 0
	movb	sys_internal_second, %al
	movl	%eax, %edi
.LVL20:
	.loc 1 68 0
	movb	sys_internal_minute, %al
.LVL21:
	movb	%al, -13(%ebp)
.LVL22:
	.loc 1 69 0
	movb	hour, %bl
.LVL23:
	.loc 1 70 0
	movb	day, %al
.LVL24:
	movb	%al, -14(%ebp)
.LVL25:
	.loc 1 71 0
	movb	month, %al
.LVL26:
	movb	%al, -15(%ebp)
.LVL27:
	.loc 1 72 0
	movl	year, %esi
.LVL28:
.L12:
	.loc 1 74 0
	call	_Z27get_update_in_progress_flagv
.LVL29:
	testl	%eax, %eax
	jne	.L12
	.loc 1 75 0
	pushl	$0
	call	_Z16get_RTC_registeri
.LVL30:
	movb	%al, sys_internal_second
	.loc 1 76 0
	movl	$2, (%esp)
	call	_Z16get_RTC_registeri
.LVL31:
	movb	%al, sys_internal_minute
	.loc 1 77 0
	movl	$4, (%esp)
	call	_Z16get_RTC_registeri
.LVL32:
	movb	%al, hour
	.loc 1 78 0
	movl	$7, (%esp)
	call	_Z16get_RTC_registeri
.LVL33:
	movb	%al, day
	.loc 1 79 0
	movl	$8, (%esp)
	call	_Z16get_RTC_registeri
.LVL34:
	movb	%al, month
	.loc 1 80 0
	movl	$9, (%esp)
	call	_Z16get_RTC_registeri
.LVL35:
	movzbl	%al, %eax
	movl	%eax, year
	.loc 1 82 0
	popl	%edx
	movl	%edi, %ecx
	cmpb	%cl, sys_internal_second
	jne	.L20
.LVL36:
	.loc 1 81 0
	movb	-13(%ebp), %cl
.LVL37:
	cmpb	%cl, sys_internal_minute
	jne	.L20
	.loc 1 81 0 is_stmt 0 discriminator 1
	cmpb	hour, %bl
	jne	.L20
	.loc 1 81 0 discriminator 2
	movb	-14(%ebp), %bl
.LVL38:
	cmpb	day, %bl
	jne	.L20
	.loc 1 82 0 is_stmt 1
	movb	-15(%ebp), %bl
	cmpb	month, %bl
	jne	.L20
	.loc 1 82 0 is_stmt 0 discriminator 1
	movl	%esi, %ecx
	movzbl	%cl, %esi
.LVL39:
	cmpl	%eax, %esi
	jne	.L20
	.loc 1 84 0 is_stmt 1
	pushl	$11
	call	_Z16get_RTC_registeri
.LVL40:
	movl	%eax, %esi
.LVL41:
	.loc 1 86 0
	popl	%eax
.LVL42:
	testl	$4, %esi
	jne	.L14
	.loc 1 87 0
	movb	sys_internal_second, %dl
	movb	%dl, %al
	shrb	$4, %al
	movb	$10, %cl
	mulb	%cl
	andl	$15, %edx
	addl	%edx, %eax
	movb	%al, sys_internal_second
	.loc 1 88 0
	movb	sys_internal_minute, %dl
	movb	%dl, %al
	shrb	$4, %al
	mulb	%cl
	andl	$15, %edx
	addl	%edx, %eax
	movb	%al, sys_internal_minute
	.loc 1 89 0
	movb	hour, %bl
	movb	%bl, %al
	shrb	$4, %al
	andl	$7, %eax
	mulb	%cl
	movb	%bl, %dl
	andl	$15, %edx
	addl	%eax, %edx
	andl	$-128, %ebx
	orl	%ebx, %edx
	movb	%dl, hour
	.loc 1 90 0
	movb	day, %dl
	movb	%dl, %al
	shrb	$4, %al
	mulb	%cl
	andl	$15, %edx
	addl	%edx, %eax
	movb	%al, day
	.loc 1 91 0
	movb	month, %dl
	movb	%dl, %al
	shrb	$4, %al
	mulb	%cl
	andl	$15, %edx
	addl	%edx, %eax
	movb	%al, month
	.loc 1 92 0
	movl	year, %eax
	movl	%eax, %edx
	shrl	$4, %edx
	imull	$10, %edx, %edx
	andl	$15, %eax
	addl	%edx, %eax
	movl	%eax, year
.L14:
	movb	hour, %al
	.loc 1 95 0
	andl	$2, %esi
.LVL43:
	jne	.L15
	.loc 1 95 0 is_stmt 0 discriminator 1
	testb	%al, %al
	jns	.L15
	.loc 1 96 0 is_stmt 1
	andl	$127, %eax
	addl	$12, %eax
	movl	$24, %ecx
	cltd
	idivl	%ecx
	movb	%dl, hour
.L15:
	.loc 1 99 0
	movl	year, %eax
	leal	2000(%eax), %edx
	.loc 1 100 0
	cmpl	$2017, %edx
	jbe	.L16
	.loc 1 99 0
	movl	%edx, year
	jmp	.L17
.L16:
	.loc 1 100 0 discriminator 1
	addl	$2100, %eax
	movl	%eax, year
.L17:
	.loc 1 101 0
	movzbl	sys_internal_second, %eax
	pushl	%eax
	movzbl	sys_internal_minute, %eax
	pushl	%eax
	movzbl	hour, %eax
	pushl	%eax
	pushl	$.LC0
	call	_Z6printfPKcz
.LVL44:
	.loc 1 102 0
	addl	$16, %esp
	leal	-12(%ebp), %esp
	popl	%ebx
	.cfi_restore 3
	popl	%esi
	.cfi_restore 6
	popl	%edi
	.cfi_restore 7
.LVL45:
	popl	%ebp
	.cfi_restore 5
	.cfi_def_cfa 4, 4
.LVL46:
	ret
	.cfi_endproc
.LFE21:
	.size	read_rtc, .-read_rtc
	.globl	year
	.section	.bss
	.align 4
	.type	year, @object
	.size	year, 4
year:
	.zero	4
	.globl	month
	.type	month, @object
	.size	month, 1
month:
	.zero	1
	.globl	day
	.type	day, @object
	.size	day, 1
day:
	.zero	1
	.globl	hour
	.type	hour, @object
	.size	hour, 1
hour:
	.zero	1
	.globl	sys_internal_minute
	.type	sys_internal_minute, @object
	.size	sys_internal_minute, 1
sys_internal_minute:
	.zero	1
	.globl	sys_internal_second
	.type	sys_internal_second, @object
	.size	sys_internal_second, 1
sys_internal_second:
	.zero	1
	.text
.Letext0:
	.file 2 "basic_lib/sys_lib.h"
	.file 3 "basic_lib/../libc/stdio.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x2bc
	.value	0x4
	.long	.Ldebug_abbrev0
	.byte	0x4
	.uleb128 0x1
	.long	.LASF28
	.byte	0x4
	.long	.LASF29
	.long	.LASF30
	.long	.Ltext0
	.long	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF0
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF1
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF2
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF3
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF4
	.uleb128 0x3
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x4
	.long	.LASF5
	.byte	0x1
	.byte	0x1a
	.long	0x33
	.uleb128 0x5
	.byte	0x3
	.long	sys_internal_second
	.uleb128 0x4
	.long	.LASF6
	.byte	0x1
	.byte	0x1b
	.long	0x33
	.uleb128 0x5
	.byte	0x3
	.long	sys_internal_minute
	.uleb128 0x4
	.long	.LASF7
	.byte	0x1
	.byte	0x1c
	.long	0x33
	.uleb128 0x5
	.byte	0x3
	.long	hour
	.uleb128 0x5
	.string	"day"
	.byte	0x1
	.byte	0x1d
	.long	0x33
	.uleb128 0x5
	.byte	0x3
	.long	day
	.uleb128 0x4
	.long	.LASF8
	.byte	0x1
	.byte	0x1e
	.long	0x33
	.uleb128 0x5
	.byte	0x3
	.long	month
	.uleb128 0x4
	.long	.LASF9
	.byte	0x1
	.byte	0x1f
	.long	0x41
	.uleb128 0x5
	.byte	0x3
	.long	year
	.uleb128 0x6
	.byte	0x7
	.byte	0x4
	.long	0x41
	.byte	0x1
	.byte	0x22
	.long	0xcf
	.uleb128 0x7
	.long	.LASF10
	.byte	0x70
	.uleb128 0x7
	.long	.LASF11
	.byte	0x71
	.byte	0
	.uleb128 0x8
	.long	.LASF31
	.byte	0x1
	.byte	0x31
	.long	.LFB21
	.long	.LFE21-.LFB21
	.uleb128 0x1
	.byte	0x9c
	.long	0x1de
	.uleb128 0x9
	.long	.LASF12
	.byte	0x1
	.byte	0x32
	.long	0x33
	.long	.LLST4
	.uleb128 0x9
	.long	.LASF13
	.byte	0x1
	.byte	0x33
	.long	0x33
	.long	.LLST5
	.uleb128 0x9
	.long	.LASF14
	.byte	0x1
	.byte	0x34
	.long	0x33
	.long	.LLST6
	.uleb128 0x9
	.long	.LASF15
	.byte	0x1
	.byte	0x35
	.long	0x33
	.long	.LLST7
	.uleb128 0x9
	.long	.LASF16
	.byte	0x1
	.byte	0x36
	.long	0x33
	.long	.LLST8
	.uleb128 0x9
	.long	.LASF17
	.byte	0x1
	.byte	0x37
	.long	0x33
	.long	.LLST9
	.uleb128 0x9
	.long	.LASF18
	.byte	0x1
	.byte	0x38
	.long	0x33
	.long	.LLST10
	.uleb128 0xa
	.long	.LVL13
	.long	0x21d
	.uleb128 0xa
	.long	.LVL14
	.long	0x1de
	.uleb128 0xa
	.long	.LVL15
	.long	0x1de
	.uleb128 0xa
	.long	.LVL16
	.long	0x1de
	.uleb128 0xa
	.long	.LVL17
	.long	0x1de
	.uleb128 0xa
	.long	.LVL18
	.long	0x1de
	.uleb128 0xa
	.long	.LVL19
	.long	0x1de
	.uleb128 0xa
	.long	.LVL29
	.long	0x21d
	.uleb128 0xa
	.long	.LVL30
	.long	0x1de
	.uleb128 0xa
	.long	.LVL31
	.long	0x1de
	.uleb128 0xa
	.long	.LVL32
	.long	0x1de
	.uleb128 0xa
	.long	.LVL33
	.long	0x1de
	.uleb128 0xa
	.long	.LVL34
	.long	0x1de
	.uleb128 0xa
	.long	.LVL35
	.long	0x1de
	.uleb128 0xa
	.long	.LVL40
	.long	0x1de
	.uleb128 0xa
	.long	.LVL44
	.long	0x29a
	.byte	0
	.uleb128 0xb
	.long	.LASF19
	.byte	0x1
	.byte	0x2c
	.long	.LASF21
	.long	0x33
	.long	.LFB20
	.long	.LFE20-.LFB20
	.uleb128 0x1
	.byte	0x9c
	.long	0x21d
	.uleb128 0xc
	.string	"reg"
	.byte	0x1
	.byte	0x2c
	.long	0x48
	.long	.LLST3
	.uleb128 0xa
	.long	.LVL10
	.long	0x2a9
	.uleb128 0xd
	.long	.LVL12
	.long	0x2b4
	.byte	0
	.uleb128 0xb
	.long	.LASF20
	.byte	0x1
	.byte	0x27
	.long	.LASF22
	.long	0x48
	.long	.LFB19
	.long	.LFE19-.LFB19
	.uleb128 0x1
	.byte	0x9c
	.long	0x24d
	.uleb128 0xa
	.long	.LVL7
	.long	0x2a9
	.uleb128 0xa
	.long	.LVL8
	.long	0x2b4
	.byte	0
	.uleb128 0xe
	.long	.LASF32
	.byte	0x1
	.byte	0x3
	.long	.LASF33
	.long	.LFB18
	.long	.LFE18-.LFB18
	.uleb128 0x1
	.byte	0x9c
	.long	0x294
	.uleb128 0xf
	.long	.LASF23
	.byte	0x1
	.byte	0x3
	.long	0x294
	.long	.LLST0
	.uleb128 0x9
	.long	.LASF24
	.byte	0x1
	.byte	0x5
	.long	0x33
	.long	.LLST1
	.uleb128 0x9
	.long	.LASF25
	.byte	0x1
	.byte	0x5
	.long	0x33
	.long	.LLST2
	.byte	0
	.uleb128 0x10
	.byte	0x4
	.long	0x33
	.uleb128 0x11
	.long	.LASF34
	.long	.LASF35
	.byte	0x3
	.byte	0x18
	.long	.LASF34
	.uleb128 0x12
	.long	.LASF26
	.long	.LASF26
	.byte	0x2
	.byte	0x11
	.uleb128 0x12
	.long	.LASF27
	.long	.LASF27
	.byte	0x2
	.byte	0x10
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
	.uleb128 0x4
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
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x5
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
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x4
	.byte	0x1
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
	.uleb128 0x7
	.uleb128 0x28
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x1c
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
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
	.uleb128 0x9
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
	.uleb128 0xa
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
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
	.uleb128 0x6e
	.uleb128 0xe
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
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0x4109
	.byte	0
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x2115
	.uleb128 0x19
	.uleb128 0x31
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
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
	.uleb128 0x6e
	.uleb128 0xe
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
	.uleb128 0xf
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
	.uleb128 0x10
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
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
	.uleb128 0x6e
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x12
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
.LLST4:
	.long	.LVL20-.Ltext0
	.long	.LVL21-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL21-.Ltext0
	.long	.LVL36-.Ltext0
	.value	0x1
	.byte	0x57
	.long	.LVL36-.Ltext0
	.long	.LVL37-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL37-.Ltext0
	.long	.LVL45-.Ltext0
	.value	0x1
	.byte	0x57
	.long	0
	.long	0
.LLST5:
	.long	.LVL22-.Ltext0
	.long	.LVL24-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL24-.Ltext0
	.long	.LVL28-.Ltext0
	.value	0x5
	.byte	0x3
	.long	sys_internal_minute
	.long	.LVL28-.Ltext0
	.long	.LVL46-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -13
	.long	.LVL46-.Ltext0
	.long	.LFE21-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -21
	.long	0
	.long	0
.LLST6:
	.long	.LVL23-.Ltext0
	.long	.LVL38-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0
	.long	0
.LLST7:
	.long	.LVL25-.Ltext0
	.long	.LVL26-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL26-.Ltext0
	.long	.LVL28-.Ltext0
	.value	0x5
	.byte	0x3
	.long	day
	.long	.LVL28-.Ltext0
	.long	.LVL46-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -14
	.long	.LVL46-.Ltext0
	.long	.LFE21-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -22
	.long	0
	.long	0
.LLST8:
	.long	.LVL27-.Ltext0
	.long	.LVL28-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL28-.Ltext0
	.long	.LVL46-.Ltext0
	.value	0x2
	.byte	0x75
	.sleb128 -15
	.long	.LVL46-.Ltext0
	.long	.LFE21-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 -23
	.long	0
	.long	0
.LLST9:
	.long	.LVL28-.Ltext0
	.long	.LVL39-.Ltext0
	.value	0x1
	.byte	0x56
	.long	.LVL39-.Ltext0
	.long	.LVL40-1-.Ltext0
	.value	0x1
	.byte	0x51
	.long	.LVL40-1-.Ltext0
	.long	.LVL41-.Ltext0
	.value	0x1
	.byte	0x56
	.long	0
	.long	0
.LLST10:
	.long	.LVL41-.Ltext0
	.long	.LVL42-.Ltext0
	.value	0x1
	.byte	0x50
	.long	.LVL42-.Ltext0
	.long	.LVL43-.Ltext0
	.value	0x1
	.byte	0x56
	.long	0
	.long	0
.LLST3:
	.long	.LVL9-.Ltext0
	.long	.LVL11-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	0
	.long	0
.LLST0:
	.long	.LVL0-.Ltext0
	.long	.LVL6-.Ltext0
	.value	0x2
	.byte	0x91
	.sleb128 0
	.long	.LVL6-.Ltext0
	.long	.LFE18-.Ltext0
	.value	0x1
	.byte	0x51
	.long	0
	.long	0
.LLST1:
	.long	.LVL3-.Ltext0
	.long	.LVL6-.Ltext0
	.value	0x1
	.byte	0x53
	.long	0
	.long	0
.LLST2:
	.long	.LVL1-.Ltext0
	.long	.LVL2-.Ltext0
	.value	0x2
	.byte	0x30
	.byte	0x9f
	.long	.LVL2-.Ltext0
	.long	.LVL4-.Ltext0
	.value	0x1
	.byte	0x52
	.long	.LVL4-.Ltext0
	.long	.LVL5-.Ltext0
	.value	0x3
	.byte	0x72
	.sleb128 1
	.byte	0x9f
	.long	.LVL6-.Ltext0
	.long	.LFE18-.Ltext0
	.value	0x1
	.byte	0x52
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
.LASF10:
	.string	"cmos_address"
.LASF20:
	.string	"get_update_in_progress_flag"
.LASF9:
	.string	"year"
.LASF6:
	.string	"sys_internal_minute"
.LASF29:
	.string	"basic_lib/cmos.cpp"
.LASF16:
	.string	"last_month"
.LASF8:
	.string	"month"
.LASF15:
	.string	"last_day"
.LASF21:
	.string	"_Z16get_RTC_registeri"
.LASF2:
	.string	"unsigned char"
.LASF34:
	.string	"_Z6printfPKcz"
.LASF14:
	.string	"last_hour"
.LASF1:
	.string	"long unsigned int"
.LASF28:
	.string	"GNU C++14 7.3.0 -m16 -march=i386 -mpreferred-stack-boundary=2 -g -Os -std=c++1z -ffreestanding -fno-builtin -fno-omit-frame-pointer -fno-tree-ch -fno-stack-protector"
.LASF33:
	.string	"_Z9read_cmosPh"
.LASF22:
	.string	"_Z27get_update_in_progress_flagv"
.LASF5:
	.string	"sys_internal_second"
.LASF32:
	.string	"read_cmos"
.LASF18:
	.string	"registerB"
.LASF13:
	.string	"last_minute"
.LASF26:
	.string	"sys_outb"
.LASF19:
	.string	"get_RTC_register"
.LASF4:
	.string	"unsigned int"
.LASF0:
	.string	"char"
.LASF7:
	.string	"hour"
.LASF11:
	.string	"cmos_data"
.LASF25:
	.string	"index"
.LASF35:
	.string	"printf"
.LASF23:
	.string	"array"
.LASF12:
	.string	"last_second"
.LASF24:
	.string	"tvalue"
.LASF31:
	.string	"read_rtc"
.LASF3:
	.string	"short unsigned int"
.LASF17:
	.string	"last_year"
.LASF27:
	.string	"sys_inb"
.LASF30:
	.string	"/Users/lixinrui/onedrive/Documents/\345\244\247\345\255\246\350\257\276\347\250\213/2018\346\223\215\344\275\234\347\263\273\347\273\237\345\256\236\351\252\214/os_lab3/test"
	.ident	"GCC: (GNU) 7.3.0"
