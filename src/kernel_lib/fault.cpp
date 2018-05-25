//
// Created by 李新锐 on 24/04/2018.
//
#include "pm.h"
extern "C" void default_fault0();
extern "C" void default_fault1();
extern "C" void default_fault2();
extern "C" void default_fault3();
extern "C" void default_fault4();
extern "C" void default_fault5();
extern "C" void default_fault6();
extern "C" void default_fault7();
extern "C" void default_fault8();
extern "C" void default_fault9();
extern "C" void default_fault10();
extern "C" void default_fault11();
extern "C" void default_fault12();
extern "C" void default_fault13();
extern "C" void default_fault14();
extern "C" void default_fault15();
extern "C" void default_fault16();
extern "C" void default_fault17();
extern "C" void default_fault18();
extern "C" void default_fault19();
extern "C" void default_fault20();
extern "C" void default_fault21();
extern "C" void default_fault22();
extern "C" void default_fault23();
extern "C" void default_fault24();
extern "C" void default_fault25();
extern "C" void default_fault26();
extern "C" void default_fault27();
extern "C" void default_fault28();
extern "C" void default_fault29();
extern "C" void default_fault30();
extern "C" void default_fault31();

void fault_init()
{
idt_install(0, (uint32_t)default_fault0, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(1, (uint32_t)default_fault1, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(2, (uint32_t)default_fault2, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(3, (uint32_t)default_fault3, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(4, (uint32_t)default_fault4, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(5, (uint32_t)default_fault5, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(6, (uint32_t)default_fault6, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(7, (uint32_t)default_fault7, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);

idt_install(8, (uint32_t)default_fault8, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(9, (uint32_t)default_fault9, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(10, (uint32_t)default_fault10, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(11, (uint32_t)default_fault11, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(12, (uint32_t)default_fault12, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(13, (uint32_t)default_fault13, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(14, (uint32_t)default_fault14, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(15, (uint32_t)default_fault15, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);

idt_install(16, (uint32_t)default_fault16, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(17, (uint32_t)default_fault17, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(18, (uint32_t)default_fault18, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(19, (uint32_t)default_fault19, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(20, (uint32_t)default_fault20, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(21, (uint32_t)default_fault21, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(22, (uint32_t)default_fault22, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(23, (uint32_t)default_fault23, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);

idt_install(24, (uint32_t)default_fault24, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(25, (uint32_t)default_fault25, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(26, (uint32_t)default_fault26, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(27, (uint32_t)default_fault27, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(28, (uint32_t)default_fault28, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(29, (uint32_t)default_fault29, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(30, (uint32_t)default_fault30, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);
idt_install(31, (uint32_t)default_fault31, SEL_KCODE << 3, GATE_INT, IDT_PR|IDT_DPL_KERN);

}