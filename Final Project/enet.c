#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"

//---------------------------------------------------------------------------------------+
// RX/TX Buffer for transfer between chips                                               |
//---------------------------------------------------------------------------------------+
byte NET_Buffer[NET_BUFFER_SIZE] = {0};
uint NET_Size = 0;

NET_MACRAW NET_Packet[NET_PACKET_BUFFER_SIZE] = {0};
uint NET_Packets = 0;

//---------------------------------------------------------------------------------------+
// Addressing information for configuration                                              |
//---------------------------------------------------------------------------------------+
typedef enum {
	ADDR_CLIENT,
	ADDR_SUBNET,
	ADDR_GATEWAY,
} addresses;
typedef enum {
	MAC_GHOST_CLIENT,
	MAC_GHOST_SERVER,
	MAC_CLIENT,
	MAC_SERVER,
} macs;

byte address[4][4] = {
	{129, 123, 5, 74}, // IP seen by client before disconnecting
	{255, 255, 254, 0}, // subnet mask, for seriously every network
	{129, 123, 5, 254}, // default gateway seen by client before disconnecting
};
byte mac[4][6] = {
	{0x00, 0x90, 0xF5, 0xE9, 0xAA, 0x21}, // Obtained via ipconfig
	{0x00, 0x12, 0xF2, 0xC2, 0x4D, 0x00}, // Obtained via wireshark
	{0x00, 0x08, 0xDC, 0x1E, 0xB8, 0x73}, // On the sticker
	{0x00, 0x08, 0xDC, 0x1E, 0xB8, 0x7D}, // On the sticker
};

//---------------------------------------------------------------------------------------+
// Parse all the data into packet groups                                                 |
//---------------------------------------------------------------------------------------+
void NET_ParsePackets() {
	uint i = 0;
	NET_Packets = 0;
	while (i < NET_Size && NET_Packets < NET_PACKET_BUFFER_SIZE) {
		// Get the packet size
		NET_Packet[NET_Packets].Size = ((NET_Buffer[i] << 8) | NET_Buffer[i + 1]) - 2;
		i += 2;
		
		// Get the contents of the Data Packet
		NET_Packet[NET_Packets].Data = NET_Buffer + i;
		NET_Packet[NET_Packets].Destination = NET_Buffer + i;
		NET_Packet[NET_Packets].Source = NET_Buffer + i + 6;
		NET_Packet[NET_Packets].Type = (NET_Buffer[i + 12] << 8) | NET_Buffer[i + 13];
		NET_Packet[NET_Packets].Payload = NET_Buffer + i + 14;
		NET_Packet[NET_Packets].PayloadSize = NET_Packet[NET_Packets].Size - 14;
		i += NET_Packet[NET_Packets].Size;
		
		// Move onto the next packet
		++NET_Packets;
	}
}

//---------------------------------------------------------------------------------------+
// Read all available data from the given chip                                           |
//---------------------------------------------------------------------------------------+
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
	
	NET_ParsePackets();
}

//---------------------------------------------------------------------------------------+
// A blind copy all into the TX buffer of a given chip                                   |
//---------------------------------------------------------------------------------------+
void NET_WRITEDATA(NET_CHIP chip){
	NET_Frame frame;
	byte data[2];
	uint i;
	
	// Constant through the function
	frame.Control.socket = 0;
	frame.Control.mode = NET_MODE_VAR;
	
	for (i = 0; i < NET_Packets; ++i) {
		// Determine how much space is available
		frame.Control.write = false;
		frame.Control.reg = NET_REG_SOCKET;
		frame.Data = data;
		frame.N = 2;
		
		// NOTE: Probably important!!!
//		//do a loop until we recieve the same ouptut**
//		frame.Address = NET_SOCKET_TX_FSR;
//		NET_SPI(chip, &frame);
//		
//		if (((frame.Data[0] << 8) + frame.Data[1]) < NET_Size){
//			//split up the data**
//		}
		
		//find the TX write pointer
		frame.Address = NET_SOCKET_TX_WR;
		NET_SPI(chip, &frame);
		
		//write the data to the buffer
		frame.Control.write = true;
		frame.Control.reg = NET_REG_TX;
		frame.Address = (data[0] << 8) | data[1];
		frame.N = NET_Packet[i].Size;
		frame.Data = NET_Packet[i].Destination;
		NET_SPI(chip, &frame);
		
		//update the TX write pointer
		data[0] = ((frame.Address + frame.N) & 0xFF00) >> 8;
		data[1] = ((frame.Address + frame.N) & 0x00FF);
		
		frame.Address = NET_SOCKET_TX_WR;
		frame.Control.reg = NET_REG_SOCKET;
		frame.Control.write = true;
		frame.Data = data;
		frame.N = 2;
		NET_SPI(chip, &frame);
		
		//Give SEND command to the CR
		frame.N = 1;
		data[0] = 0x20;
		frame.Address = NET_SOCKET_CR;
		NET_SPI(chip, &frame);
		
		// Poll to confirm the command was processed
		frame.Control.write = false;
		frame.Address = NET_SOCKET_IR;
		do { NET_SPI(chip, &frame);
		} while(!(data[0] & NET_INT_SENDOK));
		
		// Acknowledge (only) the SendOk interrupt
		frame.Control.write = true;
		data[0] = NET_INT_SENDOK;
		NET_SPI(chip, &frame);
	}
}

//---------------------------------------------------------------------------------------+
// Interrupt Handlers                                                                    |
//---------------------------------------------------------------------------------------+
byte NET_GetInterrupt(NET_CHIP chip) {
	NET_Frame frame;
	byte Int;
	
	frame.Control.mode = NET_MODE_VAR;
	frame.Control.reg = NET_REG_SOCKET;
	frame.Control.write = false;
	frame.Control.socket = 0;
	
	frame.Address = NET_SOCKET_IR;
	frame.Data = &Int;
	frame.N = 1;
	
	NET_SPI(chip, &frame);
	return Int;
}
void NET_ClearInterrupt(NET_CHIP chip, byte Int) {
	NET_Frame frame;
	frame.Control.mode = NET_MODE_VAR;
	frame.Control.reg = NET_REG_SOCKET;
	frame.Control.write = true;
	frame.Control.socket = 0;
	
	frame.Address = NET_SOCKET_IR;
	frame.Data = &Int;
	frame.N = 1;
	
	NET_SPI(chip, &frame);
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
	frame.Data = mac[MAC_GHOST_CLIENT];
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	frame.Data = mac[MAC_GHOST_SERVER];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	frame.N = 4; // Following transmissions are 4-bytes
	
	// Subnet Mask
	frame.Address = NET_COMMON_SUBNET;
	frame.Data = address[ADDR_SUBNET];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	// IP Address
	frame.Address = NET_COMMON_IP;
	frame.Data = address[ADDR_CLIENT];
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	// Look like the default gateway to the client chip
	frame.Data = address[ADDR_GATEWAY];
	NET_SPI(NET_CHIP_CLIENT, &frame);
	
	// Default Gateway
	frame.Address = NET_COMMON_GATEWAY;
	frame.Data = address[ADDR_GATEWAY];
	NET_SPI(NET_CHIP_SERVER, &frame);
	
	//---------------------------------------------------------------------------------------+
	// Common Chip Configuration                                                             |
	//---------------------------------------------------------------------------------------+
	//interrupts
	byteframe.Address = NET_COMMON_IMR;
	byteframe.Data = 0xC0; // enable interrupts for ip conflict, and dest unreachable
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Address = NET_COMMON_SIMR;
	byteframe.Data = 0xFF; // enable interrupts from all sockets
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Control.reg = NET_REG_SOCKET;
	
	// Enable interrupts on socket 0
	byteframe.Address = NET_SOCKET_IMR;
	byteframe.Data = 0xF; //enables most interrupts
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Address = NET_SOCKET_MR;
	byteframe.Data = 0x04; // MacRaw mode
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	byteframe.Data = 0x04; //0x84; // MacRaw mode w/MAC Filtering
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	byteframe.Address = NET_SOCKET_CR;
	byteframe.Data = 0x1; // Open Socket 0
	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	
	// Wait for the sockets to finish opening
	byteframe.Address = NET_SOCKET_SR;
	byteframe.Control.write = false;
	
	do { NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
	} while (byteframe.Data != 0x42);
	
	do { NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
	} while (byteframe.Data != 0x42);
}

/*
// Set socket 0 to use the full buffer
//	byteframe.Data = 0;
//	for (i = 1; i < 8; ++i) {
//		byteframe.Control.socket = i;
//		byteframe.Address = NET_SOCKET_RXBUF_SIZE;
//		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//		
//		byteframe.Address = NET_SOCKET_TXBUF_SIZE;
//		NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//		NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	}
//	byteframe.Control.socket = 0;
//	
//	byteframe.Data = 16;
//	byteframe.Address = NET_SOCKET_RXBUF_SIZE;
//	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
//	
//	byteframe.Address = NET_SOCKET_TXBUF_SIZE;
//	NET_SPI_BYTE(NET_CHIP_CLIENT, &byteframe);
//	NET_SPI_BYTE(NET_CHIP_SERVER, &byteframe);
*/
