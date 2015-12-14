#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"

void NET_READDATA(NET_CHIP chip, NET_Frame* frame, unsigned short datasize){
	static byte* data;
	static byte data2[2];
	//static unsigned short readsize;
	
	frame->Control.write = false;
	frame->Control.reg = NET_REG_SOCKET;

	frame->Address = NET_SOCKET_RX_RSR;
	frame->N = 2;
	do{
	datasize = (frame->Data[0] << 8) + frame->Data[1];
	NET_SPI(chip, frame);
	} while (datasize != ((frame->Data[0] << 8) + frame->Data[1]));
	
	
	//find the RX read pointer
	frame->Address = NET_SOCKET_RX_RD;
	NET_SPI(chip, frame);
	
	//read the data on the buffer
	frame->Address = (frame->Data[0] << 8) + frame->Data[1];
	frame->Control.reg = NET_REG_RX;
	frame->N = datasize;
	NET_SPI(chip, frame);
	
	data = frame->Data;
	
	//update the RX read pointer
	data2[0] = (frame->Address + datasize) >> 8;
	data2[1] = (frame->Address + datasize) & 0xFF;
	
	frame->Address = NET_SOCKET_RX_RD;
	frame->Control.reg = NET_REG_SOCKET;
	frame->Control.write = true;
	frame->Data = data2;
	frame->N = 2;
	NET_SPI(chip, frame);
	
	//Give RECV command to the CR
	frame->Address = NET_SOCKET_CR;
	data2[0] = 0x40;
	frame->N = 1;
	NET_SPI(chip, frame);
	//we may need to poll at some point to confirm that the command was processed!!!
	
	frame->Data = data;
}

void NET_WRITEDATA(NET_CHIP chip, NET_Frame* frame, unsigned short datasize){
	static byte* data;
	static byte data2[2];
	//static unsigned short readsize;
	
	frame->Control.write = false;
	frame->Control.reg = NET_REG_SOCKET;

	frame->Address = NET_SOCKET_TX_FSR;
	data = frame->Data;
	frame->Data = data2;
	frame->N = 2;
	
	//do a loop until we recieve the same ouptut**
	NET_SPI(chip, frame);
	
	if (((frame->Data[0] << 8) + frame->Data[1]) < datasize){
		//split up the data**
	}
	
	//find the TX write pointer
	frame->Address = NET_SOCKET_RX_WR;
	NET_SPI(chip, frame);
	
	//write the data to the buffer
	frame->Address = (frame->Data[0] << 8) + frame->Data[1];
	frame->Control.reg = NET_REG_TX;
	frame->N = datasize;
	frame->Control.write = true;
	frame->Data = data;
	NET_SPI(chip, frame);
	
	//update the TX write pointer
	data2[0] += datasize >> 8;
	data2[1] += datasize & 0xFF;
	
	frame->Address = NET_SOCKET_TX_WR;
	frame->Control.reg = NET_REG_SOCKET;
	frame->Control.write = true;
	frame->Data = data2;
	frame->N = 2;
	NET_SPI(chip, frame);
	
	//Give SEND command to the CR
	frame->Address = NET_SOCKET_CR;
	data2[0] = 0x20;
	frame->N = 1;
	NET_SPI(chip, frame);
	//we may need to poll at some point to confirm that the command was processed!!!
	
	frame->Data = data;
}

void NET_PARSEDATA(byte* data, unsigned short datasize){
	
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
	uint i;
	SPI_Frame spi;
	byte
		mosi[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		miso[20] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	
	// Safety percautions
	if (frame->N > 17) { return false; }
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
	spi.N = frame->N + 3;
	spi.MOSI = mosi;
	spi.MISO = miso;
	spi.CS = (chip == NET_CHIP_CLIENT)
		? &NET_CS_CLIENT
		: &NET_CS_SERVER;
	
	// Copy data to MOSI when writing
	if (frame->Control.write) {
		for (i = 0; i < frame->N; ++i) {
			mosi[i + 3] = frame->Data[i];
		}
	}
	
	// Perform the SPI transaction
	SPI_Transfer(SSI0, &spi);
	
	// Copy data from MISO when reading
	if (!frame->Control.write) {
		for (i = 0; i < frame->N; ++i) {
			frame->Data[i] = miso[i + 3];
		}
	}
	
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
		ADDR_CLIENTIP,
		ADDR_GATEWAY,
	} addresses;
	byte address[4][4] = {
		{192, 168, 0, 1}, // default ip address in most systems
		{255, 255, 255, 0}, // subnet mask, for seriously every network
		{129, 123, 4, 128}, // ip address as seen by client before disconnecting
		{129, 123, 5, 254}, // default gateway as seen by client before disconnecting
	};
	typedef enum {
		MAC_GHOST,
		MAC_CLIENT,
		MAC_SERVER,
	} macs;
	byte mac[3][6] = {
		{0x00, 0x23, 0xAE, 0x6F, 0x11, 0xC4},
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
	
	// Debug code
	frame.Data = debug;
	frame.Control.write = false;
	
	frame.N = 1;
	frame.Address = 0x39;
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	frame.N = 4;
	
	frame.Control.write = true;
	
	//---------------------------------------------------------------------------------------+
	// Chip Addressing Configuration                                                         |
	//---------------------------------------------------------------------------------------+
	// MAC (physical address)
	frame.N = 6;
	frame.Address = NET_COMMON_MAC;
	frame.Data = mac[MAC_CLIENT];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	
	frame.Data = mac[MAC_GHOST];
	NET_SPI(NET_CHIP_SERVER, &frame);
	frame.N = 4; // Following transmissions are 4-bytes
	
	//ip address
	frame.Address = NET_COMMON_IP;
	frame.Data = address[ADDR_GATEWAY]; // client chip pretends to be the default gateway
	NET_SPI(NET_CHIP_CLIENT, &frame);
	frame.Data = address[ADDR_CLIENTIP]; // server chip pretends to be the client
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
	
//	byteframe.Control.reg = NET_REG_SOCKET;
//	byteframe.Address = NET_SOCKET_IMR;
//	byteframe.Data = 0x4; //enables "recieving from peer" interrupt
//	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	for (i = 0; i < 8; ++i) {
//		byteframe.Control.socket = i;
//		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	}
	
	//OPEN socket 0, OPEN command is done last
//	byteframe.Address = NET_SOCKET_CR;
//	byteframe.Data = 0x1; //OPEN socket command
//	byteframe.Control.socket = 0;
//	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	for (i = 0; i < 8; ++i) {
//		byteframe.Control.socket = i;
//		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	}
}
