#include <sys/defs.h>
#include <sys/gdt.h>
#include <sys/kprintf.h>
#include <sys/tarfs.h>
#include <sys/ahci.h>
#include <sys/idt.h>
#include <sys/homework.h>
#include <sys/hwfont.h>

#define INITIAL_STACK_SIZE 4096
uint8_t initial_stack[INITIAL_STACK_SIZE]__attribute__((aligned(16)));
uint32_t* loader_stack;
extern char kernmem, physbase;

void start(uint32_t *modulep, void *physbase, void *physfree)
{
 while(modulep[0] != 0x9001) modulep += modulep[1]+2;
	setToGraphicalMode();
	clearScreen();
	hwfont("Hino\n");
	hwfont("Namaste\n");
	hwfont("This character %c\n", 'p');
	hwfont("One %d\n", 133);
	hwfont("This string %s\n", "Amazing");
	hwfont("133 to hexa is %x\n", 133);
	hwfont("Pointer %p\n", 133);
	hwfont("something\n");
	hwfont("another thing\n");
	hwfont("Lets keep typing\n");
	hwfont("Yes baby yes\n");
	hwfont("This will be the last line\n");
	hwfont("This will send Hino home\n");
  while(1) ;
}

void boot(void)
{
  // note: function changes rsp, local stack variables can't be practically used
  register char *temp1, *temp2;

  for(temp2 = (char*)0xb8001; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = 7 /* white */;
  __asm__ volatile (
    "cli;"
    "movq %%rsp, %0;"
    "movq %1, %%rsp;"
    :"=g"(loader_stack)
    :"r"(&initial_stack[INITIAL_STACK_SIZE])
  );
  init_gdt();
  for(temp2 = (char*)0xb8000; temp2 < (char*)0xb8000+160*25; temp2 += 2) *temp2 = ' ' /* white */;
  start(
    (uint32_t*)((char*)(uint64_t)loader_stack[3] + (uint64_t)&kernmem - (uint64_t)&physbase),
    (uint64_t*)&physbase,
    (uint64_t*)(uint64_t)loader_stack[4]
  );
  for(
    temp1 = "!!!!! start() returned !!!!!", temp2 = (char*)0xb8000;
    *temp1;
    temp1 += 1, temp2 += 2
  ) *temp2 = *temp1;
  while(1) __asm__ volatile ("hlt");
}
