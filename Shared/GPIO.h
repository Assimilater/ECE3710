#ifndef _GPIO_H_
#define _GPIO_H_
#include "embedded_t.h"
#include "TM4C123GH6PM.h"

//access register via bit, byte, halfword, and word
typedef union {
	struct {
		volatile unsigned
			bit0:1,  bit1:1,  bit2:1,  bit3:1,  bit4:1,  bit5:1,  bit6:1,  bit7:1,
			bit8:1,  bit9:1,  bit10:1, bit11:1, bit12:1, bit13:1, bit14:1, bit15:1,
			bit16:1, bit17:1, bit18:1, bit19:1, bit20:1, bit21:1, bit22:1, bit23:1,
			bit24:1, bit25:1, bit26:1, bit27:1, bit28:1, bit29:1, bit30:1, bit31:1;
	};
	struct {
		volatile unsigned
			nibble0:4, nibble1:4,
			nibble2:4, nibble3:4,
			nibble4:4, nibble5:4,
			nibble6:4, nibble7:4;
	};
	volatile byte byte[4];
	volatile uint16 half[2];
	volatile uint word;
} REG;

typedef struct {
	volatile byte DATA_[1019]; //TI bit addresses
	REG DATA; //0x3FC
	REG DIR; //0x400
	REG IS; //0x404
	REG IBE; //0x408
	REG IEV; //0x40C
	REG IM; //0x410
	REG RIS; //0x414
	REG MIS; //0x418
	REG ICR; //0x41C
	REG AFSEL; //0x420
	volatile byte RES[220]; //reserved
	REG DR2R; //0x500
	REG DR4R; //0x504
	REG DR8R; //0x508
	REG ODR; //0x50C
	REG PUR; //0x510
	REG PDR; //0x514
	REG SLR; //0x518
	REG DEN; //0x51C
	REG LOCK; //0x520
	REG CR; //0x524
	REG AMSEL; // 0x528
	REG PCTL; // 0x52C
	REG ADCCTL; // 0x530
} GPIOPort;

typedef struct {
	GPIOPort* PortA;
	GPIOPort* PortB;
	GPIOPort* PortC;
	GPIOPort* PortD;
	GPIOPort* PortE;
	GPIOPort* PortF;
} GPIOBoard;

const static GPIOBoard GPIO = {
	(GPIOPort*) GPIOA_BASE,
	(GPIOPort*) GPIOB_BASE,
	(GPIOPort*) GPIOC_BASE,
	(GPIOPort*) GPIOD_BASE,
	(GPIOPort*) GPIOE_BASE,
	(GPIOPort*) GPIOF_BASE
};

#endif
