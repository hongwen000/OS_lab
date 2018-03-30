//template<int N>
//constexpr int fibonacci()
//{
//    if constexpr (N>=2)
//        return fibonacci<N-1>() + fibonacci<N-2>();
//    else
//        return N;
//}
extern "C" void kernel_main()
{
    //sys_bios_clear_screen();
    //tty_init();
    // int x = 0;
    // int y = 5;
    // int offset = 2*(x * 80 + y);
    // int color = MAKE_COLOR(VGA_BLACK, VGA_WHITE);
    // int c = 'A';
    // int ch = c | (color << 8);
    // //sys_bios_print_int(ch, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS(2,10));
    // //sys_bios_print_int(offset, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS(3,10));
    // asm volatile (".intel_syntax noprefix\n\t"
    // "pusha\n\t"
    // "mov cx, 0xB800\n\t"
    // "mov gs, cx\n\t"
	// "mov bp,bx\n\t"
    //  "mov word ptr gs:[bp],ax\n\t"
    // "popa\n\t"
    // ".att_syntax noprefix\n\t"
    // :
    // :"a"(ch),"b"(offset)
    // :"%ecx");
    //sys_putchar('\t', MAKE_COLOR(VGA_BLACK, VGA_WHITE), 0, 0);
    //tty tty1;
    //tty1.tty_init();
    //current_tty = &tty1;
    //while(1){
    //    int ch = getchar();
    //    sys_print_int(ch, 10, 10);
    //    putchar(ch);
    //}
    //int year = 1998;
    //sys_bios_print_int(year, MAKE_COLOR(VGA_BLACK, VGA_YELLOW), MAKE_POS(1,0));
    //int fib = fibonacci<20>();
    //sys_bios_print_int(fib, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS(2,0));
    //char arr[] = "Hello, world";
    //sys_bios_print_string(arr, 12, MAKE_COLOR(VGA_BLACK, VGA_YELLOW), MAKE_POS(0, 0));
    //int i = 1;
    //while(i++) {
    //    int num = sys_get_scancode();
    //    sys_bios_print_int(num, MAKE_COLOR(VGA_BLACK, VGA_WHITE), MAKE_POS((5+i),5));
    //}
}