#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"

void NET_PARSEDATA() {
	
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
// The ControlByte object is not packing properly, so this is a fix for byte packing     |
//---------------------------------------------------------------------------------------+
byte NET_ControlFix(ControlByte cb) {
	byte value = 0;
	value |= cb.socket << 5;
	value |= cb.reg << 3;
	value |= cb.write << 2;
	value |= cb.mode;
	return value;
}

//---------------------------------------------------------------------------------------+
// Alias the more generic NET_SPI to handle case of single byte transmission             |
//---------------------------------------------------------------------------------------+
bool NET_SPI_BYTE(NET_CHIP chip, NET_Byteframe* frame) {
	NET_Frame rogue;
	rogue.N = 1;
	rogue.Address = frame->Address;
	rogue.Control = frame->Control;
	rogue.Data = &frame->Data;
	return NET_SPI(chip, &rogue);
}
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
	NET_CS_ISP = 0;
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
	SSI0->DR = NET_ControlFix(frame->Control);
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
		SSI0->DR = write;
	}
	while (SSI0->SR & 0x10); // Wait for BSY == 0
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
	SSI0->DR = NET_ControlFix(frame->Control);
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

//---------------------------------------------------------------------------------------+
// Initialize ip addressing to a ghost configuration                                     |
//---------------------------------------------------------------------------------------+
void NET_Init() {
	uint i;
	NET_Frame frame;
	NET_Byteframe byteframe;
	typedef enum {
		ADDR_HOME,
		ADDR_SUBNET,
		ADDR_CLIENTIP,
		ADDR_GATEWAY,
	} addresses;
	byte address[4][4] = {
		{192, 168, 0, 1}, // default ip address in most systems
		{255, 255, 255, 0}, // subnet mask, for seriously every network
		{129, 123, 85, 134}, // ip address as seen by client before disconnecting
		{129, 123, 85, 254}, // default gateway as seen by client before disconnecting
	};
	typedef enum {
		MAC_CLIENT,
		MAC_CPS,
		MAC_ISP,
	} macs;
	byte mac[3][6] = {
		{0xB8, 0xAC, 0x6F, 0xA4, 0xAD, 0x42},
		{0x00, 0x08, 0xDC, 0x1E, 0xB8, 0x73},
		{0x00, 0x08, 0xDC, 0x1E, 0xB8, 0x7D},
	};
	byte debug[6];
	
	// Reset the chips
	// doc: http://wizwiki.net/wiki/doku.php?id=products:wiz550io:allpages#reset_timing
	// Busy wait to keep reset pin low for longer than 400us
	NET_RST = 0;
	for (i = 0; i < 5000; ++i);
	NET_RST = 1;
	
	while (!NET_RDY_CPC); // Wait for ready signal from CPC
	while (!NET_RDY_ISP); // Wait for ready signal from ISP
	
	// Initialize the frame for all configuration needs
	frame.Control.write = true;
	frame.Control.reg = NET_REG_COMMON;
	frame.Control.mode = NET_MODE_VAR;
	frame.Control.socket = 0;
	
	// Initialize the byteframe for all configuration needs
	byteframe.Control.write = true;
	byteframe.Control.reg = NET_REG_COMMON;
	byteframe.Control.mode = NET_MODE_VAR;
	byteframe.Control.socket = 0;
	
	//---------------------------------------------------------------------------------------+
	// Chip Addressing Configuration                                                         |
	//---------------------------------------------------------------------------------------+
	// MAC (physical address)
	frame.N = 6;
	frame.Address = NET_COMMON_MAC;
	
	frame.Data = mac[MAC_CPS];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	
	frame.Data = mac[MAC_CLIENT];
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	// Following transmissions are 4-bytes
	frame.N = 4;
	
	//ip address
	frame.Address = NET_COMMON_IP;
	frame.Data = address[ADDR_GATEWAY]; // pretends to be the default gateway
	NET_SPI(NET_CHIP_CLIENT, &frame);
	
	frame.Data = address[ADDR_CLIENTIP]; // pretends to be the client
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	//subnet mask
	frame.Address = NET_COMMON_SUBNET;
	frame.Data = address[ADDR_SUBNET];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	//default gateway
	frame.Address = NET_COMMON_GATEWAY;
	frame.Data = address[ADDR_HOME];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	
	frame.Data = address[ADDR_GATEWAY];
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	// Debug code
	frame.Data = debug;
	frame.Control.write = false;
	
	frame.Address = NET_COMMON_IP;
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	frame.Address = NET_COMMON_GATEWAY;
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	frame.Address = NET_COMMON_SUBNET;
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	frame.N = 6;
	frame.Address = NET_COMMON_MAC;
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	frame.N = 4;
	frame.Control.write = true;
	
	//---------------------------------------------------------------------------------------+
	// Common Chip Configuration                                                             |
	//---------------------------------------------------------------------------------------+
	//interrupts
	byteframe.Address = NET_COMMON_SIMR;
	byteframe.Data = 0x1; //enable interrupts from socket 0
	byteframe.Control.write = true;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Control.write = false;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Control.write = true;
	
	byteframe.Control.reg = NET_REG_SOCKET;
	byteframe.Address = NET_SOCKET_IMR;
	byteframe.Data = 0x4; //enables "recieving from peer" interrupt
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	for (i = 0; i < 8; ++i) {
//		byteframe.Control.socket = i;
//		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	}
	
	//OPEN all sockets
	byteframe.Address = NET_SOCKET_CR;
	byteframe.Data = 0x1; //OPEN socket command
	byteframe.Control.socket = 0;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	for (i = 0; i < 8; ++i) {
//		byteframe.Control.socket = i;
//		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	}

	byteframe.Address = NET_COMMON_IR;
	byteframe.Control.reg = NET_REG_COMMON;
	byteframe.Control.write = false;
	byteframe.Control.socket = 0;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);

	byteframe.Address = NET_COMMON_IMR;
	byteframe.Control.reg = NET_REG_COMMON;
	byteframe.Control.write = false;
	byteframe.Control.socket = 0;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);


	byteframe.Address = NET_COMMON_SIR;
	byteframe.Control.reg = NET_REG_COMMON;
	byteframe.Control.write = false;
	byteframe.Control.socket = 0;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	for (i = 0; i < 8; ++i) {
		byteframe.Address = NET_SOCKET_SIR;
		byteframe.Control.reg = NET_REG_SOCKET;
		byteframe.Control.write = false;
		byteframe.Control.socket = i;
		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	}
}
