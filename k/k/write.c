#include "io.h"
#include "write.h"
#define COM1 0x3f8
#define _8_bits_no_parity_dlab_on 0x83
#define DLL 0x03
#define DLM 0x00
#define FIFO 0xc7
#define _8_bits_no_parity_dlab_off 0x03


void init_serial()
{

    //COM1+3
    outb(COM1+3,_8_bits_no_parity_dlab_on);

    //Vitesse
    //DLL
    outb(COM1,DLL);
    //DLM
    outb(COM1,DLM);
    //COM1+2 
    outb(COM1 + 2,FIFO);

    outb(COM1 + 3,_8_bits_no_parity_dlab_off);

}


int write(const char *buf, size_t count)
{
	for(int i =0; buf[i]; i++)
		outb(COM1, buf[i]);
	return count;
}



