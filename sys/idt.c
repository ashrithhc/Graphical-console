#include <sys/defs.h>
#include <sys/kprintf.h>
#include <sys/idt.h>

static void init_idt();
static void idt_set_gate(uint8_t, uint64_t, uint16_t, uint8_t);

static struct idt_entry_struct idt_entries[256];
static struct idt_ptr_struct   idt_ptr;

void isr0(){
    kprintf("isr0\n");
}

void isr32(){
/*kprintf("----------");	
   __asm__ __volatile__ ("pushq %%rax" : : :);
    __asm__ __volatile__ ("pushq %%rcx; pushq %%rdx; pushq %%rsi; pushq %%rdi; pushq %%r8; pushq %%r9; pushq %%r10; pushq %%r11" : : :);
  */  __asm__ __volatile__ ("cli");
/*
    kprintf("isr32\n");
__asm__ __volatile__ ("popq %%r11; popq %%r10; popq %%r9; popq %%r8; popq %%rdi; popq %%rsi; popq %%rdx; popq %%rcx; popq %%rax" : : :);
*/__asm__ __volatile__ ("outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint64_t)0x20));
__asm__ __volatile__ ("STI");
    // outb(0x20,0x20);
    // __asm__ __volatile__ ("outb %0, %1" : : "a"(0x20), "Nd"(0x20));
    __asm__("iretq");
}

void isr33(){
	kprintf("----------");
    __asm__ __volatile__ ("pushq %%rax" : : :);
    __asm__ __volatile__ ("pushq %%rcx; pushq %%rdx; pushq %%rsi; pushq %%rdi; pushq %%r8; pushq %%r9; pushq %%r10; pushq %%r11" : : :);
    __asm__ __volatile__ ("CLI");
    uint8_t ret;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (ret) : "Nd" ((uint64_t)0x60));
    kprintf("isr33 - %d\n", ret);
    __asm__ __volatile__ ("popq %%r11; popq %%r10; popq %%r9; popq %%r8; popq %%rdi; popq %%rsi; popq %%rdx; popq %%rcx; popq %%rax" : : :);
    __asm__ __volatile__ ("outb %0, %1" : : "a"((uint8_t)0x20), "Nd"((uint64_t)0x20));
    __asm__ __volatile__ ("STI");
}

void init_descriptor_tables()
{
    init_idt();
}

static void init_idt()
{
	kprintf("init_idt printing\n");
    //idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 -1;
    //idt_ptr.base  = (uint64_t)idt_entries;

   for(int i=0; i<32; i++){
//    idt_set_gate( i, (uint64_t)&isr0 , 0x08, 0x8E);
    }

    idt_set_gate(32, (uint64_t)&isr32, 0x08, 0x8E);
    idt_set_gate(33, (uint64_t)&isr33, 0x08, 0x8E);

    for(int i = 34; i<256; i++){
  // 	idt_set_gate( i, (uint64_t)&isr0 , 0x08, 0x8E);
    }
    idt_ptr.limit = sizeof(struct idt_entry_struct) * 256 -1;
    idt_ptr.base  = (uint64_t)idt_entries;
    __asm__ ("lidt (%0)"::"r"(&idt_ptr));
}

static void idt_set_gate(uint8_t num, uint64_t base, uint16_t sel, uint8_t typeAttr)
{
    idt_entries[num].offset_low = base & 0xFFFF;
    idt_entries[num].offset_mid = (base >> 16) & 0xFFFF;
    idt_entries[num].offset_high = (base >> 32) & 0xFFFFFFFF;

    idt_entries[num].sel = 0x08;
    idt_entries[num].zero0 = 0;
    idt_entries[num].zero1 = 0;
    idt_entries[num].typeAttr = 0x8E;
}

