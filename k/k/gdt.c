#include <k/kstd.h>
#include <string.h>
#include "multiboot.h"
#include "io.h"

#include "gdt.h"

struct gdt_r gdtr;
struct gdt_entry gdt[5];



// Set the value of one GDT entry

void init_gdt( u32 base, u32 limite, u8 A_AVL_DB_G, u8 gran_type, s32 index)
{
    /* Setup the descriptor limits */

     gdt[index].lim0_15 = (limite & 0xffff);
     gdt[index].base0_15 = (base & 0xffff);
     gdt[index].base16_23 = (base & 0xff0000) >> 16;
     gdt[index].granularity_type = (gran_type & 0xf0);
     gdt[index].lim16_19 = (limite & 0xf00000) >> 16; 
     gdt[index].A_AVL_DB_G = A_AVL_DB_G; 

    /* Setup the descriptor base access */
     gdt[index].base24_31 = (base & 0xff000000) >> 24;

    /* Finally, set up the granularity and access flags */





}



//Initialise the global descriptor table.

void load(void)
{
  memset(gdt,0x0,5);
  init_gdt(0x0, 0x0, 0x0, 0x0, 0);      /* Null segment*/
  init_gdt(0x0, 0xffffffff, 0xC, 0x9A, 1); /* kernel code */
  init_gdt(0x0, 0xffffffff, 0xC, 0x93, 2); /* kernel data */
  //init_gdt(0x0, 0xfffff, 0xff, 0x0d, &gdt[3]);    /* user code */
  //init_gdt(0x0, 0xfffff, 0xF3, 0x0d, &gdt[4]);   /* user data */

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
