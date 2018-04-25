//
// Created by 李新锐 on 25/04/2018.
//
#include "./proc.h"
#include "../libc/stdio.h"
PCB pcb1;
void schedule(PCB *ppcb) {
    pcb1.gs = ppcb->gs;
    pcb1.fs  = ppcb->fs;
    pcb1.es = ppcb->es;
    pcb1.ds = ppcb->ds;
    pcb1.edi = ppcb->edi;
    pcb1.esi = ppcb->esi;
    pcb1.ebp = ppcb->ebp;
    pcb1.esp = ppcb->esp;
    pcb1.ebx = ppcb->ebx;
    pcb1.edx = ppcb->edx;
    pcb1.ecx = ppcb->ecx;
    pcb1.eax = ppcb->eax;
    pcb1.eip = ppcb->eip;
    pcb1.cs = ppcb->cs;
    pcb1.eflags = ppcb->eflags;
    pcb1.user_esp = ppcb->user_esp;
    pcb1.ss = ppcb->ss;
    pcb1.gs = pcb1.gs & 0xFFFF;
    pcb1.fs = pcb1.fs & 0xFFFF;
    pcb1.es = pcb1.es & 0xFFFF;
    pcb1.ds = pcb1.ds & 0xFFFF;
    pcb1.cs = pcb1.cs & 0xFFFF;
    pcb1.ss = pcb1.ss & 0xFFFF;
    printf("gs: 0x%x\n",pcb1.gs);    // 16 bits
    printf("fs: 0x%x\n",pcb1.fs);    // 16 bits
    printf("es: 0x%x\n",pcb1.es);    // 16 bits
    printf("ds: 0x%x\n",pcb1.ds);          // 16 bits
    printf("edi: 0x%x\n",pcb1.edi);
    printf("esi: 0x%x\n",pcb1.esi);
    printf("ebp: 0x%x\n",pcb1.ebp);
    printf("esp: 0x%x\n",pcb1.esp);
    printf("ebx: 0x%x\n",pcb1.ebx);
    printf("edx: 0x%x\n",pcb1.edx);
    printf("ecx: 0x%x\n",pcb1.ecx);
    printf("eax: 0x%x\n",pcb1.eax);
    printf("eip: 0x%x\n",pcb1.eip);
    printf("cs: 0x%x\n",pcb1.cs);    // 16 bits
    printf("eflags: 0x%x\n",pcb1.eflags);
    printf("user_esp: 0x%x\n",pcb1.user_esp);
    printf("ss: 0x%x\n",pcb1.ss);    // 16 bits
}

