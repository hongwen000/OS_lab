;.section .init
;	/* gcc will nicely put the contents of crtend.o's .init section here. */
;	popl %ebp
;	ret
;
;.section .fini
;	/* gcc will nicely put the contents of crtend.o's .fini section here. */
;	popl %ebp
;	ret

[BITS 32]
[section .init]
    pop ebp
    ret

[section .fini]
    pop ebp
    ret