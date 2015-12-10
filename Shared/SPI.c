#include "SPI.h"

void SPI_Wait(SSI0_Type* SSI) {
	while (!(SSI->SR & 0x1)); // Wait for TFE = 1
	while (SSI->SR & 0x10); // Wait for BSY == 0
}
byte SPI_Clear(SSI0_Type* SSI) {
	byte read;
	SPI_Wait(SSI);
	while (SSI->SR & 0x4) {
		read = SSI->DR;
	}
	return read;
}

void SPI_Transfer(SSI0_Type* SSI, SPI_Frame* frame) {
	uint i, j, r;
	SPI_Clear(SSI);
	*frame->CS = 0;
	
	// Sending in groups of 8 (SPI FIFO Buffer Size)
	i = 0;
	for (r = frame->N; r > 8; r -= 8) {
		for (j = 0; j < 8; ++j) {
			SSI->DR = frame->MOSI[i + j];
		}
		SPI_Wait(SSI);
		for (j = 0; j < 8; ++j) {
			frame->MISO[i + j] = SSI->DR;
		}
		i += 8;
	}
	
	// Send all remaining groups
	for (j = 0; j < r; ++j) {
		SSI->DR = frame->MOSI[i + j];
	}
	SPI_Wait(SSI);
	for (j = 0; j < r; ++j) {
		frame->MISO[i + j] = SSI->DR;
	}
	
	*frame->CS = 1;
}
