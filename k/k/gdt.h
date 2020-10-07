#ifndef GDT_H
#define GDT_H
#include <stddef.h>
#include <string.h>
#include <k/kstd.h>

#define TYPE_S_DPL_P_CODE 0x9b //1001 1010
#define TYPE_S_DPL_P_DATA 0x93 //1001 0011
#define AVL_L_DB_G 0xdf //1100 1111
#define ON 0x01
#define OFF 0x00
#define BASE 0x0

struct gdt_entry
{
    u16 limit_15;
    u8 limit_3 : 4;
    u16 base_address_15;
    u8 base_address_7;
    u8 base_address;
    u8 type_s_dpl_p;
    u8 avl_l_db_g : 4;
}__attribute__((packed));

struct gdt_register
{
    u16 limit;
    u32 base_address;
}__attribute__((packed));

void gdt_init();

void load_cr0_segment_selector_and_cs();

#endif
