#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"

void NET_READDATA(NET_CHIP chip) {
	NET_Frame frame;
	byte data[2];
	uint16 size = 0;
	
	// Constant through the function
	frame.Control.socket = 0;
	frame.Control.mode = NET_MODE_VAR;
	
	// Determine the amount of data to read
	frame.Control.reg = NET_REG_SOCKET;
	frame.Control.write = false;
	frame.Address = NET_SOCKET_RX_RSR;
	frame.Data = data;
	frame.N = 2;
	do{
		NET_Size = size;
		NET_SPI(chip, &frame);
		size = (data[0] << 8) | data[1];
	} while (NET_Size != size);
	
	// find the RX read pointer
	frame.Address = NET_SOCKET_RX_RD;
	NET_SPI(chip, &frame);
	
	// read the data on the buffer
	frame.Control.reg = NET_REG_RX;
	frame.Address = (data[0] << 8) | data[1];
	frame.Data = NET_Buffer;
	frame.N = NET_Size;
	NET_SPI(chip, &frame);
	
	//update the RX read pointer
	data[0] = ((frame.Address + NET_Size) & 0xFF00) >> 8;
	data[1] = ((frame.Address + NET_Size) & 0x00FF);
	
	frame.Control.write = true;
	frame.Control.reg = NET_REG_SOCKET;
	frame.Address = NET_SOCKET_RX_RD;
	frame.Data = data;
	frame.N = 2;
	NET_SPI(chip, &frame);
	
	//Give RECV command to the CR
	frame.Address = NET_SOCKET_CR;
	data[0] = 0x40;
	frame.N = 1;
	NET_SPI(chip, &frame);
	
	//we may need to poll at some point to confirm that the command was processed!!!
}

void NET_WRITEDATA(NET_CHIP chip){
	NET_Frame frame;
	byte data[2];
	
	// Constant through the function
	frame.Control.socket = 0;
	frame.Control.mode = NET_MODE_VAR;
	
	// Determine how much space is available
	frame.Control.write = false;
	frame.Control.reg = NET_REG_SOCKET;
	frame.Address = NET_SOCKET_TX_FSR;
	frame.Data = data;
	frame.N = 2;
	
	//do a loop until we recieve the same ouptut**
	NET_SPI(chip, &frame);
	
	if (((frame.Data[0] << 8) + frame.Data[1]) < NET_Size){
		//split up the data**
	}
	
	//find the TX write pointer
	frame.Address = NET_SOCKET_RX_WR;
	NET_SPI(chip, &frame);
	
	//write the data to the buffer
	frame.Control.write = true;
	frame.Control.reg = NET_REG_TX;
	frame.Address = (data[0] << 8) | data[1];
	frame.N = NET_Size;
	frame.Data = NET_Buffer;
	NET_SPI(chip, &frame);
	
	//update the TX write pointer
	data[0] = ((frame.Address + NET_Size) & 0xFF00) >> 8;
	data[1] = ((frame.Address + NET_Size) & 0x00FF);
	
	frame.Address = NET_SOCKET_TX_WR;
	frame.Control.reg = NET_REG_SOCKET;
	frame.Control.write = true;
	frame.Data = data;
	frame.N = 2;
	NET_SPI(chip, &frame);
	
	//Give SEND command to the CR
	frame.Address = NET_SOCKET_CR;
	data[0] = 0x20;
	frame.N = 1;
	NET_SPI(chip, &frame);
	//we may need to poll at some point to confirm that the command was processed!!!
}

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
// Converts address into bytestream, does input validation, and calls spi handler        |
//---------------------------------------------------------------------------------------+
bool NET_SPI(NET_CHIP chip, NET_Frame* frame) {
	SPI_Frame spi;
	byte mosi[3] = {0},
		miso[3] = {0};
	
	// Safety percautions
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
	if (frame->Control.reg == NET_REG_COMMON) {
		frame->Control.socket = 0;
	}
	
	// Type conversion
	mosi[0] = (frame->Address & 0xFF00) >> 8;
	mosi[1] = (frame->Address & 0x00FF);
	mosi[2] = NET_ControlFix(frame->Control);
	
	// Setup the spi frame
	spi.bus.N = 3;
	spi.bus.MOSI = mosi;
	spi.bus.MISO = miso;
	spi.route.SSI = SSI0;
	spi.route.CS = (chip == NET_CHIP_CLIENT)
		? &NET_CS_CLIENT
		: &NET_CS_SERVER;
	
	// Fancy SPI_Transfer
	SPI_Begin(&spi.route);
	
	// Send the Address/Control Block
	SPI_Block(&spi.route, &spi.bus);
	
	// Send the data block
	spi.bus.N = frame->N;
	if (frame->Control.write) {
		spi.bus.MISO = 0;
		spi.bus.MOSI = frame->Data;
	} else {
		spi.bus.MISO = frame->Data;
		spi.bus.MOSI = 0;
	}
	SPI_Block(&spi.route, &spi.bus);
	
	// End Fancy SPI_Transfer
	SPI_End(&spi.route);
	
	return true;
}

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
		ADDR_GATEWAY,
	} addresses;
	byte address[4][4] = {
		{192, 168, 0, 1}, // default ip address in most systems
		{255, 255, 255, 0}, // subnet mask, for seriously every network
		{129, 123, 5, 254}, // default gateway as seen by client before disconnecting
	};
	typedef enum {
		MAC_GHOST,
		MAC_CLIENT,
		MAC_SERVER,
	} macs;
	byte mac[3][6] = {
		{0x00, 0x90, 0xF5, 0xE9, 0xAA, 0x21},
		{0x00, 0x08, 0xDC, 0x1E, 0xB8, 0x73},
		{0x00, 0x08, 0xDC, 0x1E, 0xB8, 0x7D},
	};
	
	// Reset the chips
	// doc: http://wizwiki.net/wiki/doku.php?id=products:wiz550io:allpages#reset_timing
	// Busy wait to keep reset pin low for longer than 400us
	NET_RST = 0;
	for (i = 0; i < 5000; ++i);
	NET_RST = 1;
	while (!NET_RDY_CLIENT); // Wait for ready signal from CPC
	while (!NET_RDY_SERVER); // Wait for ready signal from ISP
	
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
	frame.Data = mac[MAC_GHOST];
	NET_SPI(NET_CHIP_SERVER, &frame);
	frame.N = 4; // Following transmissions are 4-bytes
	
	//ip address
	frame.Address = NET_COMMON_IP;
	frame.Data = address[ADDR_HOME];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	
	//default gateway
	frame.Data = address[ADDR_GATEWAY];
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	//---------------------------------------------------------------------------------------+
	// Common Chip Configuration                                                             |
	//---------------------------------------------------------------------------------------+
	//interrupts
	byteframe.Address = NET_COMMON_IMR;
	byteframe.Data = 0xC0; // enable interrupts for ip conflict, and dest unreachable
	byteframe.Control.write = true;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Address = NET_COMMON_SIMR;
	byteframe.Data = 0xFF; // enable interrupts from all sockets
	byteframe.Control.write = true;
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	// Enable interrupts on all sockets
	byteframe.Control.reg = NET_REG_SOCKET;
	byteframe.Address = NET_SOCKET_IMR;
	byteframe.Data = 0xF; //enables most interrupts
	//for (i = 0; i < 8; ++i) {
		byteframe.Control.socket = 0; // i;
		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	//}
	
	byteframe.Control.socket = 0;
	byteframe.Address = NET_SOCKET_MR;
	byteframe.Data = 4; // MacRaw mode
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Address = NET_SOCKET_CR;
	byteframe.Data = 0x1; // Open Socket 0
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	// Wait for the sockets to finish opening
	byteframe.Address = NET_SOCKET_SR;
	byteframe.Control.write = false;
	while (byteframe.Data != 0x42) {
		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	}
	while (byteframe.Data != 0x42) {
		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	}
}
