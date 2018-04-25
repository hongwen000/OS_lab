//
// Created by 李新锐 on 25/04/2018.
//
#include "./proc.h"
#include "../libc/stdio.h"

int used_pcb_num = 0;
PCB *pcb_que = nullptr;
PCB pcb3;
PCB pcb2;
PCB pcb1;
PCB pcb0;
PCB* pcb[MAX_PROC] {
        &pcb0,
        &pcb1,
        &pcb2,
        &pcb3
};
CPU_INFO info;
void proc_init()
{
    pcb0.id = 0;
    pcb1.id = 1;
    pcb2.id = 2;
    pcb3.id = 3;

    pcb0.next = nullptr;
    pcb1.next = nullptr;
    pcb2.next = nullptr;
    pcb3.next = nullptr;

    pcb_que = &pcb0;
    used_pcb_num = 0;
}
void get_pcb()
{
    if (used_pcb_num == MAX_PROC) return;
    if (used_pcb_num != 0) pcb[used_pcb_num - 1]->next = pcb[used_pcb_num];
    pcb[used_pcb_num]->next = pcb[0];
    pcb_que = pcb[used_pcb_num];
    ++used_pcb_num;
}

void save(CPU_INFO *cpu_info) {
    CPU_INFO info;
    memcpy(&info, cpu_info, sizeof(CPU_INFO));
    if (!pcb_que)return;
    pcb_que->gs = cpu_info->gs;
    pcb_que->fs  = cpu_info->fs;
    pcb_que->es = cpu_info->es;
    pcb_que->ds = cpu_info->ds;
    pcb_que->edi = cpu_info->edi;
    pcb_que->esi = cpu_info->esi;
    pcb_que->ebp = cpu_info->ebp;
    pcb_que->esp = cpu_info->esp;
    pcb_que->ebx = cpu_info->ebx;
    pcb_que->edx = cpu_info->edx;
    pcb_que->ecx = cpu_info->ecx;
    pcb_que->eax = cpu_info->eax;
    pcb_que->eip = cpu_info->eip;
    pcb_que->cs = cpu_info->cs;
    pcb_que->eflags = cpu_info->eflags;

    pcb_que->gs = pcb_que->gs & 0xFFFF;
    pcb_que->fs = pcb_que->fs & 0xFFFF;
    pcb_que->es = pcb_que->es & 0xFFFF;
    pcb_que->ds = pcb_que->ds & 0xFFFF;
    pcb_que->cs = pcb_que->cs & 0xFFFF;
    printf("Addr of pcb: %x\n", (uint32_t) pcb_que);
    printf("gs: 0x%x\n",pcb_que->gs);    // 16 bits
    printf("fs: 0x%x\n",pcb_que->fs);    // 16 bits
    printf("es: 0x%x\n",pcb_que->es);    // 16 bits
    printf("ds: 0x%x\n",pcb_que->ds);          // 16 bits
    printf("edi: 0x%x\n",pcb_que->edi);
    printf("esi: 0x%x\n",pcb_que->esi);
    printf("ebp: 0x%x\n",pcb_que->ebp);
    printf("esp: 0x%x\n",pcb_que->esp);
    printf("ebx: 0x%x\n",pcb_que->ebx);
    printf("edx: 0x%x\n",pcb_que->edx);
    printf("ecx: 0x%x\n",pcb_que->ecx);
    printf("eax: 0x%x\n",pcb_que->eax);
    printf("eip: 0x%x\n",pcb_que->eip);
    printf("cs: 0x%x\n",pcb_que->cs);    // 16 bits
    printf("eflags: 0x%x\n",pcb_que->eflags);
}
void round_robin()
{
    pcb_que=pcb_que->next;
}

