#ifndef _SYS_H
#define _SYS_H

#include "gd32f4xx.h"
#include "systick.h"


#define BIT_ADDR(byte_offset,bit_number) (volatile unsigned long*)(0x42000000 +(byte_offset << 5)+(bit_number << 2))
	
#define GPIOA_OCTL_OFFSET ((GPIOA + 0x14) - 0x40000000)
#define GPIOD_OCTL_OFFSET ((GPIOD + 0x14) - 0x40000000)

#define PAout(n)  *(BIT_ADDR(GPIOA_OCTL_OFFSET,n))
#define PDout(n)  *(BIT_ADDR(GPIOD_OCTL_OFFSET,n))


#endif