#ifndef __SPI_H__
#define __SPI_H__

#include "Controller.h"

typedef struct {
	M4_DMA CS;
	SSI0_Type* SSI;
} SPI_Route;
typedef struct {
	byte* MOSI;
	byte* MISO;
	uint N;
} SPI_Bus;
typedef struct {
	SPI_Route route;
	SPI_Bus bus;
} SPI_Frame;

void SPI_Transfer(SPI_Frame*);
void SPI_Begin(SPI_Route*);
void SPI_Block(SPI_Route*, SPI_Bus*);
void SPI_End(SPI_Route*);

#endif
