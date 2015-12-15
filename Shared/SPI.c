#include "SPI.h"

//---------------------------------------------------------------------------------------+
// Forward declaration of function that actually handles the SPI FIFO buffers            |
//---------------------------------------------------------------------------------------+
void SPI_ReadWrite(SPI_Route*, SPI_Bus*);
void SPI_Read(SPI_Route*, SPI_Bus*);
byte SPI_Write(SPI_Route*, SPI_Bus*);

//---------------------------------------------------------------------------------------+
// Small helpers that are used often when working with SPI                               |
//---------------------------------------------------------------------------------------+
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
	
	// return value to get rid of compiler warning (#lazy)
	return read;
}

//---------------------------------------------------------------------------------------+
// Publicly accessible functions                                                         |
//---------------------------------------------------------------------------------------+
void SPI_Transfer(SPI_Frame* frame) {
	SPI_Begin(&frame->route);
	SPI_Block(&frame->route, &frame->bus);
	SPI_End(&frame->route);
}
void SPI_Begin(SPI_Route* route) {
	SPI_Clear(route->SSI);
	*route->CS = 0;
}
void SPI_Block(SPI_Route* route, SPI_Bus* bus) {
	if (!bus->MISO) {
		if (!bus->MOSI) { return; }
		SPI_Write(route, bus);
	} else if (!bus->MOSI) {
		SPI_Read(route, bus);
	} else {
		SPI_ReadWrite(route, bus);
	}
}
void SPI_End(SPI_Route* route) {
	*route->CS = 1;
}

//---------------------------------------------------------------------------------------+
// ReadWrite is the standard, read and write handle null pointer cases                   |
//---------------------------------------------------------------------------------------+
void SPI_ReadWrite(SPI_Route* route, SPI_Bus* bus) {
	uint i, j, r;
	
	// Sending in groups of 8 (SPI FIFO Buffer Size)
	i = 0;
	for (r = bus->N; r > 8; r -= 8) {
		for (j = 0; j < 8; ++j) {
			route->SSI->DR = bus->MOSI[i + j];
		}
		SPI_Wait(route->SSI);
		for (j = 0; j < 8; ++j) {
			bus->MISO[i + j] = route->SSI->DR;
		}
		i += 8;
	}
	
	// Send all remaining groups
	for (j = 0; j < r; ++j) {
		route->SSI->DR = bus->MOSI[i + j];
	}
	SPI_Wait(route->SSI);
	for (j = 0; j < r; ++j) {
		bus->MISO[i + j] = route->SSI->DR;
	}
}
void SPI_Read(SPI_Route* route, SPI_Bus* bus) {
	uint i, j, r;
	byte dummy = 0;
	
	// Sending in groups of 8 (SPI FIFO Buffer Size)
	i = 0;
	for (r = bus->N; r > 8; r -= 8) {
		for (j = 0; j < 8; ++j) {
			route->SSI->DR = dummy;
		}
		SPI_Wait(route->SSI);
		for (j = 0; j < 8; ++j) {
			bus->MISO[i + j] = route->SSI->DR;
		}
		i += 8;
	}
	
	// Send all remaining groups
	for (j = 0; j < r; ++j) {
		route->SSI->DR = dummy;
	}
	SPI_Wait(route->SSI);
	for (j = 0; j < r; ++j) {
		bus->MISO[i + j] = route->SSI->DR;
	}
}
byte SPI_Write(SPI_Route* route, SPI_Bus* bus) {
	uint i, j, r;
	byte dummy;
	
	// Sending in groups of 8 (SPI FIFO Buffer Size)
	i = 0;
	for (r = bus->N; r > 8; r -= 8) {
		for (j = 0; j < 8; ++j) {
			route->SSI->DR = bus->MOSI[i + j];
		}
		SPI_Wait(route->SSI);
		for (j = 0; j < 8; ++j) {
			dummy = route->SSI->DR;
		}
		i += 8;
	}
	
	// Send all remaining groups
	for (j = 0; j < r; ++j) {
		route->SSI->DR = bus->MOSI[i + j];
	}
	SPI_Wait(route->SSI);
	for (j = 0; j < r; ++j) {
		dummy = route->SSI->DR;
	}
	
	// return value to get rid of compiler warning (#lazy)
	return dummy;
}
