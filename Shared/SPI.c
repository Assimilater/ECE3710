#include "SPI.h"

void SPI_Wait() {
	while (!(SSI0->SR & 0x1)); // Wait for TFE = 1
	while (SSI0->SR & 0x10); // Wait for BSY == 0
}
byte SPI_Clear() {
	byte read;
	SPI_Wait();
	while (SSI0->SR & 0x4) {
		read = SSI0->DR;
	}
	return read;
}

void SPI_Transfer(SPI_Frame* frame) {
	uint i, j, r;
	SPI_Clear();
	*frame->CS = 0;
	
	// Sending in groups of 8 (SPI FIFO Buffer Size)
	i = 0;
	for (r = frame->N; r > 8; r -= 8) {
		for (j = 0; j < 8; ++j) {
			SSI0->DR = frame->MOSI[i + j];
		}
		SPI_Wait();
		for (j = 0; j < 8; ++j) {
			frame->MISO[i + j] = SSI0->DR;
		}
		i += 8;
	}
	
	// Send all remaining groups
	for (j = 0; j < r; ++j) {
		SSI0->DR = frame->MOSI[i + j];
	}
	SPI_Wait();
	for (j = 0; j < r; ++j) {
		frame->MISO[i + j] = SSI0->DR;
	}
	
	*frame->CS = 1;
}
