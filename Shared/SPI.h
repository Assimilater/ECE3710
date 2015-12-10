#ifndef __SPI_H__
#define __SPI_H__

#include "Controller.h"

typedef struct {
	M4_DMA CS;
	byte* MOSI;
	byte* MISO;
	uint N;
} SPI_Frame;

void SPI_Transfer(SSI0_Type*, SPI_Frame*);

#endif
