;.section .init
;.global _init
;.type _init, @function
;_init:
;	push %ebp
;	movl %esp, %ebp
;	/* gcc will nicely put the contents of crtbegin.o's .init section here. */
;
;.section .fini
;.global _fini
;.type _fini, @function
;_fini:
;	push %ebp
;	movl %esp, %ebp
;	/* gcc will nicely put the contents of crtbegin.o's .fini section here. */
[BITS 32]
[section .init]
[global _init]
;db 'Here is init segment'
_init:
    push ebp
    mov ebp, esp

[section .fini]
[global _fini]
_fini:
    push ebp
    mov ebp, esp
