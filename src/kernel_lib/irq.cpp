/* irq.c 
 * This file is modified form Bram's Kernel Development Tutorial
 * Handle the Interrupt Requests(IRQs)h raised by hardware device 
 */
// std
#include "../include/defines.h"
#include "../kernel_lib/sys_utility.h"
// x86
#include "isr.h"
#include "pic.h"
#include "pm.h"
#include "debug_printf.h"

void pic_init(){
    sys_outb(PIC1_CMD, ICW1_INIT + ICW1_ICW4);    // starts the initialization sequence (in cascade mode)
    sys_outb(PIC2_CMD, ICW1_INIT + ICW1_ICW4);
}

void irq_clear_mask(){
    sys_outb(PIC1_DATA, 0x0);                   // clear irq maske, enable all irq in Mister PIC
    sys_outb(PIC2_DATA, 0x0);                   // clear irq maske, enable all irq in Slave PIC
}

/* map irq 0-15 to int 32-47 */
void irq_remap(){
    pic_init();

    sys_outb(PIC1_DATA, 0x20);                   // ICW2: Master PIC vector offset
    sys_outb(PIC2_DATA, 0x28);                   // ICW2: Slave PIC vector offset

    sys_outb(PIC1_DATA, 4);                      // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
    sys_outb(PIC2_DATA, 2);                      // ICW3: tell Slave PIC its cascade identity (0000 0010)
 
    sys_outb(PIC1_DATA, ICW4_8086);
    sys_outb(PIC2_DATA, ICW4_8086);
 
    irq_clear_mask();
}

extern "C" void default_irq0();
extern "C" void default_irq1();
extern "C" void default_irq2();
extern "C" void default_irq3();
extern "C" void default_irq4();
extern "C" void default_irq5();
extern "C" void default_irq6();
extern "C" void default_irq7();
extern "C" void default_irq8();
extern "C" void default_irq9();
extern "C" void default_irq10();
extern "C" void default_irq11();
extern "C" void default_irq12();
extern "C" void default_irq13();
extern "C" void default_irq14();
extern "C" void default_irq15();

void irq_init(){
    irq_remap();
    debug_printf("IRQ INIT OK");

    idt_install(ISR_IRQ0 + 0, (uint32_t)default_irq0, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 1, (uint32_t)default_irq1, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 2, (uint32_t)default_irq2, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 3, (uint32_t)default_irq3, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 4, (uint32_t)default_irq4, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 5, (uint32_t)default_irq5, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 6, (uint32_t)default_irq6, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 7, (uint32_t)default_irq7, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 8, (uint32_t)default_irq8, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 9, (uint32_t)default_irq9, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 10, (uint32_t)default_irq10, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 11, (uint32_t)default_irq11, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 12, (uint32_t)default_irq12, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 13, (uint32_t)default_irq13, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 14, (uint32_t)default_irq14, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
    idt_install(ISR_IRQ0 + 15, (uint32_t)default_irq15, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
}

