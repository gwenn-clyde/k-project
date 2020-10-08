#include "gdt.h"

struct gdt_entry gdt_table[5];
struct gdt_register gdtr;

void gdt_set(struct gdt_entry *gdt, u32 base, u32 limit,
    u8 type_s_dpl_p, u8 avl_l_db_g)
{
    gdt->limit_15 = limit & 0xffff;
    gdt->base_address_15 = base & 0xffff;
    gdt->base_address_7 = (base & 0xff0000) >> 16;
    gdt->type_s_dpl_p = type_s_dpl_p;
    gdt->limit_3 = (limit & 0xf0000) >> 16;
    gdt->avl_l_db_g = avl_l_db_g;
    gdt->base_address = (base & 0xff000000) >> 24;
}

void gdt_init()
{
    memset(gdt_table, 0x0, 5);
    gdt_set(&gdt_table[1], BASE, 0xfffff, TYPE_S_DPL_P_CODE, AVL_L_DB_G);
    gdt_set(&gdt_table[2], BASE, 0xfffff, TYPE_S_DPL_P_DATA, AVL_L_DB_G);

    gdtr.base_address = (u32)gdt_table;
    gdtr.limit = sizeof(gdt_table) - 1;

        asm volatile("lgdt %0\n"
        : /* no output */
        : "m" (gdtr)
        : "memory");
}

void load_cr0_segment_selector_and_cs()
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
