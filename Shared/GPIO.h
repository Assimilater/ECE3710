#ifndef _GPIO_H_
#define _GPIO_H_
#include "embedded_t.h"
#include "TM4C123GH6PM.h"

typedef struct {
	volatile unsigned char DATA_[1019]; //TI bit addresses
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
	volatile unsigned char RES[220]; //reserved
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
