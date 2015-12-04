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
	char read;
	//CS goes low
	
	NET_WriteCmd(0x0017,0x01); //Socket Interrupt Register, read 1 byte
	//Data Phase
	read = SSI0->DR;
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
void NET_SPI_Client(byte*, NET_Frame);
void NET_SPI_Server(byte*, NET_Frame);
void NET_SPI_Read(byte*, NET_Frame);
byte NET_SPI_Write(byte*, NET_Frame);

//---------------------------------------------------------------------------------------+
// Converts address into bytestream, does input validation, helper prep and delegation   |
//---------------------------------------------------------------------------------------+
void NET_SPI(NET_CHIP_SELECT chip, NET_Frame frame) {
	uint i;
	byte read, address[2];
	
	// Type conversion
	address[0] = (frame.Address & 0xFF00) >> 8;
	address[1] = (frame.Address & 0x00FF);
	
	// Safety percautions
	if (frame.Control.reg == NET_COMMON_R) {
		frame.Control.socket = 0;
	}
	
	// Clear the buffer, just in case
	while (SSI0->SR & 0x4) { read = SSI0->DR; }
	while(!(SSI0->SR & 0x1)); // Wait for TFE = 1
	
	// Chip interaction over SPI
	if (chip == NET_CHIP_CLIENT) {
		NET_SPI_Client(address, frame);
	} else {
		NET_SPI_Server(address, frame);
	}
}

//---------------------------------------------------------------------------------------+
// Handles toggling the CS for the client and calls Read|Write                           |
//---------------------------------------------------------------------------------------+
void NET_SPI_Client(byte* address, NET_Frame frame) {
	NET_CS_CPC = 0;
	if (frame.Control.write) {
		NET_SPI_Write(address, frame);
	} else {
		NET_SPI_Read(address, frame);
	}
	NET_CS_CPC = 1;
}
//---------------------------------------------------------------------------------------+
// Handles toggling the CS for the server and calls Read|Write                           |
//---------------------------------------------------------------------------------------+
void NET_SPI_Server(byte* address, NET_Frame frame) {
	NET_CS_CPC = 1;
	if (frame.Control.write) {
		NET_SPI_Write(address, frame);
	} else {
		NET_SPI_Read(address, frame);
	}
	NET_CS_ISP = 1;
}

//---------------------------------------------------------------------------------------+
// Writes dummy variable, reads into frame.Data                                          |
//---------------------------------------------------------------------------------------+
void NET_SPI_Read(byte* address, NET_Frame frame) {
	uint i, j, k;
	byte write;
	
	// Initialize the SPI Frame (See section 2.2 in w550 datasheet)
	SSI0->DR = address[0];
	SSI0->DR = address[1];
	SSI0->DR = frame.Control.byte;
	while(SSI0->SR & 0x10); // Wait for BSY == 0
	write = SSI0->DR;
	write = SSI0->DR;
	write = SSI0->DR;
	write = 0; // Reading in don't cares, make sure we write 0
	
	// See Figure 12 in 2550 datasheet for frame example
	for (i = frame.N; i > 8; i -= 8) {
		// Our SPI FIFO buffer is limited to 8 blocks
		for (j = 0; j < 8; ++j) {
			SSI0->DR = write;
		}
		while(SSI0->SR & 0x10); // Wait for BSY == 0
		k = frame.N - i;
		for (j = 0; j < 8; ++j) {
			frame.Data[k + j] = SSI0->DR;
		}
	}
	
	// Read all remaining blocks
	while(SSI0->SR & 0x10); // Wait for BSY == 0
	k = frame.N - i;
	for (j = 0; j < i; ++j) {
		frame.Data[k + j] = SSI0->DR;
	}
}
//---------------------------------------------------------------------------------------+
// Writes frame.Data, reads into dummy variable                                          |
//---------------------------------------------------------------------------------------+
byte NET_SPI_Write(byte* address, NET_Frame frame) {
	uint i, j;
	byte read;
	
	// Initialize the SPI Frame (See section 2.2 in w550 datasheet)
	SSI0->DR = address[0];
	SSI0->DR = address[1];
	SSI0->DR = frame.Control.byte;
	
	// See Figure 10 in 2550 datasheet for frame example
	for (i = 0; i < frame.N; ++i) {
		// Our SPI FIFO buffer is limited to 8 blocks
		if (i % 8 == 0) {
			while(SSI0->SR & 0x10); // Wait for BSY == 0
			for (j = 0; j < 8; ++j) {
				read = SSI0->DR;
			}
		}
		SSI0->DR = frame.Data[i];
	}
	
	// Read all remaining blocks
	while(SSI0->SR & 0x10); // Wait for BSY == 0
	for (i %= 8; i > 0; --i) {
		read = SSI0->DR;
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
