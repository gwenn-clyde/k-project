#ifndef GDT_H
#define GDT_H

struct gdt_entry{

    u16 lim0_15;
    u16 base0_15;
    u8  base16_23;
    u8  granularity_type;
    u8  lim16_19;
    u8  A_AVL_DB_G;
    u8  base24_31;
}__attribute__((packed));


struct gdt_r{
    u16 limit;
    u32 base;
}__attribute__((packed));

void load(void);
void load_cr0_seg_and_cs();
#endif
