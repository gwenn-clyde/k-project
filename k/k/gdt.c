#include <k/kstd.h>
#include <string.h>
#include "multiboot.h"
#include "io.h"

#include "gdt.h"

struct gdt_r gdtr;
struct gdt_entry gdt[5];



// Set the value of one GDT entry

void init_gdt( u32 base, u32 limite, u8 access, u8 gran, struct gdt_entry *p)
{
    /* Setup the descriptor base access */

     p->base0_15 = (base & 0xffff);
     p->base16_23 = (base & 0xff0000) >> 16;
     p->base24_31 = (base & 0xff000000) >> 24;

    /* Setup the descriptor limits */

     p->lim0_15 = (limite & 0xffff);

    /* Finally, set up the granularity and access flags */

     p->granularity = (limite & 0xff0000) >> 16;
     p->granularity |= (gran & 0xf0);
     p->access = access;

}



//Initialise the global descriptor table.

void load(void)
{
  memset(gdt,0x0,5);
  init_gdt(0x0, 0x0, 0x0, 0x0, &gdt[0]);      /* Null segment*/
  init_gdt(0x0, 0xfffff, 0x9b, 0x0d, &gdt[1]); /* kernel code */
  init_gdt(0x0, 0xfffff, 0x93, 0x0d, &gdt[2]); /* kernel data */
  init_gdt(0x0, 0xfffff, 0xff, 0x0d, &gdt[3]);    /* user code */
  init_gdt(0x0, 0xfffff, 0xF3, 0x0d, &gdt[4]);   /* user data */

  gdtr.base = (u32)&gdt;
  gdtr.limit = sizeof(gdt) - 1;

    asm volatile("lgdt %0\n"
    : /* no output */
    : "m" (gdtr)
    : "memory");
}

void load_cr0_seg_and_cs()
{
    __asm__(
    "   movl %cr0, %eax\n"
    "   or $1, %eax\n" //only enable protection set
    "   movl %eax, %cr0\n"
    "   movw $0x10, %ax\n" // 2 << 3 = 16 = 0x10
    "   movw %ax, %ss\n"
    "   movw %ax, %ds\n"
    "   movw %ax, %es\n"
    "   movw %ax, %fs\n"
    "   movw %ax, %gs\n"
    "   pushl $0x08\n" // 1 << 3 = 8 = 0x08
    "   pushl $1f\n"
    "   lret\n"
    "   1:\n"
    );
}
