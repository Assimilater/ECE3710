#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"

void NET_WriteCmd(short address, char control) {
	//Phase descriptions start at PG.18 in data sheet
	//Address Phase
	SSI0->DR = address >> 8 & 0xFF;
	SSI0->DR = address & 0xFF;
	//Control Phase  //BSB selects block //0bBSB[4:0]RWB[0]OM[1:0]
	SSI0->DR = control;
}

void NET_READDATA() {
	//char read;
	//CS goes low
	
	NET_WriteCmd(0x0017,0x01); //Socket Interrupt Register, read 1 byte
	//Data Phase
	//read = SSI0->DR;
	//CS goes high
	
	//check what socket
	
	//cs goes low
	//NET_WriteCmd();
	//read
	//cs goes high
	
}

void NET_PARSEDATA() {
	
}

void NET_WRITEDATA() {
	NET_WriteCmd(0x0000,0x00);
	
	//Data Phase
	//Write
	
	//After putting data in a socket's tx buffer a SEND command must be given through the socket command register
}

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// SECTION - NET_SPI: Generic function handler with helpers for w550io SPI interfacing   |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
void NET_SPI_Client(byte*, NET_Frame*);
void NET_SPI_Server(byte*, NET_Frame*);
void NET_SPI_Read(byte*, NET_Frame*);
void NET_SPI_Write(byte*, NET_Frame*);

//---------------------------------------------------------------------------------------+
// Converts address into bytestream, does input validation, helper prep and delegation   |
//---------------------------------------------------------------------------------------+
bool NET_SPI(NET_CHIP chip, NET_Frame* frame) {
	byte address[2];
	
	// Clear the buffer, just in case
	while (!(SSI0->SR & 0x1)); // Wait for TFE = 1
	while (SSI0->SR & 0x10); // Wait for BSY == 0
	while (SSI0->SR & 0x4) { address[0] = SSI0->DR; }
	
	// Safety percautions
	if (frame->Control.reg == NET_REG_COMMON) {
		frame->Control.socket = 0;
	}
	if (frame->Control.mode == NET_MODE_F1B) {
		if (frame->N > 1) { frame->N = 1; }
		else if (frame->N < 1) { return false; }
	}
	if (frame->Control.mode == NET_MODE_F2B) {
		if (frame->N > 2) { frame->N = 2; }
		else if (frame->N < 2) { return false; }
	}
	if (frame->Control.mode == NET_MODE_F4B) {
		if (frame->N > 4) { frame->N = 4; }
		else if (frame->N < 4) { return false; }
	}
	
	// Type conversion
	address[0] = (frame->Address & 0xFF00) >> 8;
	address[1] = (frame->Address & 0x00FF);
	
	// Chip interaction over SPI
	if (chip == NET_CHIP_CLIENT) {
		NET_SPI_Client(address, frame);
	} else {
		NET_SPI_Server(address, frame);
	}
	
	return true;
}

//---------------------------------------------------------------------------------------+
// Handles toggling the CS for the CLIENT and calls Read|Write                           |
//---------------------------------------------------------------------------------------+
void NET_SPI_Client(byte* address, NET_Frame* frame) {
	NET_CS_CPC = 0;
	if (frame->Control.write) {
		NET_SPI_Write(address, frame);
	} else {
		NET_SPI_Read(address, frame);
	}
	NET_CS_CPC = 1;
}
//---------------------------------------------------------------------------------------+
// Handles toggling the CS for the SERVER and calls Read|Write                           |
//---------------------------------------------------------------------------------------+
void NET_SPI_Server(byte* address, NET_Frame* frame) {
	NET_CS_CPC = 1;
	if (frame->Control.write) {
		NET_SPI_Write(address, frame);
	} else {
		NET_SPI_Read(address, frame);
	}
	NET_CS_ISP = 1;
}

//---------------------------------------------------------------------------------------+
// Writes dummy variable, reads into frame->Data                                         |
//---------------------------------------------------------------------------------------+
void NET_SPI_Read(byte* address, NET_Frame* frame) {
	uint i, j, k;
	byte write;
	
	// Initialize the SPI Frame (See section 2.2 in w550 datasheet)
	SSI0->DR = address[0];
	SSI0->DR = address[1];
	SSI0->DR = frame->Control.byte;
	while (SSI0->SR & 0x10); // Wait for BSY == 0
	write = SSI0->DR;
	write = SSI0->DR;
	write = SSI0->DR;
	write = 0; // Reading in don't cares, make sure we write 0
	
	// See Figure 12 in 2550 datasheet for frame example
	for (i = frame->N; i > 8; i -= 8) {
		// Our SPI FIFO buffer is limited to 8 blocks
		for (j = 0; j < 8; ++j) {
			SSI0->DR = write;
		}
		while (SSI0->SR & 0x10); // Wait for BSY == 0
		k = frame->N - i;
		for (j = 0; j < 8; ++j) {
			frame->Data[k + j] = SSI0->DR;
		}
	}
	
	// Read all remaining blocks
	while (SSI0->SR & 0x10); // Wait for BSY == 0
	k = frame->N - i;
	for (j = 0; j < i; ++j) {
		frame->Data[k + j] = SSI0->DR;
	}
}
//---------------------------------------------------------------------------------------+
// Reads dummy variable, writes into frame->Data                                         |
//---------------------------------------------------------------------------------------+
void NET_SPI_Write(byte* address, NET_Frame* frame) {
	uint i;
	
	// Initialize the SPI Frame (See section 2.2 in w550 datasheet)
	SSI0->DR = address[0];
	SSI0->DR = address[1];
	SSI0->DR = frame->Control.byte;
	// Since we don't care about reading data, it's ok if the Rx buffer overflows
	
	// See Figure 10 in 2550 datasheet for frame example
	for (i = 0; i < frame->N; ++i) {
		while(!(SSI0->SR & 0x2)); // Wait for TNF == 1
		SSI0->DR = frame->Data[i];
	}
	
	// Empty the FIFO buffer
	while (SSI0->SR & 0x10); // Wait for BSY == 0
	while (SSI0->SR & 0x4) { // While RNE == 1
		i = SSI0->DR; // We're done with i, so read in don't care
	}
}

//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+
// END SECTION: NET_SPI                                                                  |
//---------------------------------------------------------------------------------------+
//---------------------------------------------------------------------------------------+

void NET_Init() {
	NET_CS_CPC = 1;
	NET_CS_ISP = 1;
	
}
