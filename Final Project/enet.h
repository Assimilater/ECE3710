#ifndef __WIZ550_H__
#define __WIZ550_H__

//---------------------------------------------------------------------------------------+
// Include Dependencies                                                                  |
//---------------------------------------------------------------------------------------+
#include "../Shared/embedded_t.h"
#include "../Shared/SPI.h"

//---------------------------------------------------------------------------------------+
// Hepler Structs For Configuration                                                      |
//---------------------------------------------------------------------------------------+
typedef enum {
	NET_CHIP_CLIENT,
	NET_CHIP_SERVER
} NET_CHIP;
typedef enum {
	NET_REG_COMMON,
	NET_REG_SOCKET,
	NET_REG_TX,
	NET_REG_RX
} NET_REG;
typedef enum {
	NET_MODE_VAR,
	NET_MODE_F1B,
	NET_MODE_F2B,
	NET_MODE_F4B
} NET_MODE;
typedef union {
	struct {
		uint
			socket:3, // Note: Must be 0 if using common register
			reg:2, // Default: NET_REG_COMMON
			write:1, // Default: Read (false)
			mode:2; // Default: NET_VAR_M
	};
	byte byte;
} ControlByte; // See section 2.2.2 in w550 datasheet

typedef struct {
	uint16 Address;
	ControlByte Control;
	byte* Data;
	uint N;
} NET_Frame;
typedef struct {
	uint16 Address;
	ControlByte Control;
	byte Data;
} NET_Byteframe;

typedef union {
	byte byte[4];
} IPv4;
typedef union {
	byte byte[6];
} MAC;

typedef enum {
	NET_INT_CON =		1 << 0,
	NET_INT_DISCON =	1 << 1,
	NET_INT_RECV =		1 << 2,
	NET_INT_TIMEOUT =	1 << 3,
	NET_INT_SENDOK =	1 << 4,
} NET_INT;

typedef struct {
	uint16 Size; // Data Packet Size
	byte* Data; // Pointer to "Data Packet" (same as Destination)
	byte* Destination; // size 6
	byte* Source; // size 6
	uint16 Type;
	
	uint16 PayloadSize;
	byte* Payload; // size "Size - 14"
} NET_MACRAW;

static const uint NET_BUFFER_SIZE = 2000;
extern byte NET_Buffer[NET_BUFFER_SIZE];
extern uint NET_Size;

static const uint NET_PACKET_BUFFER_SIZE = 50;
extern NET_MACRAW NET_Packet[NET_PACKET_BUFFER_SIZE];
extern uint NET_Packets;

//---------------------------------------------------------------------------------------+
// Driver Functions (specific to interpreting macraw data and blocking requests)         |
//---------------------------------------------------------------------------------------+
void NET_READDATA(NET_CHIP);
void NET_WRITEDATA(NET_CHIP);
void NET_SENDRESET(NET_CHIP);
bool NET_CHECKBLOCK(void);

//---------------------------------------------------------------------------------------+
// Driver Functions                                                                      |
//---------------------------------------------------------------------------------------+
byte NET_GetInterrupt(NET_CHIP);
void NET_ClearInterrupt(NET_CHIP, byte);
bool NET_SPI_BYTE(NET_CHIP, NET_Byteframe*);
bool NET_SPI(NET_CHIP, NET_Frame*);
void NET_Init(void);

//---------------------------------------------------------------------------------------+
// Communication signals for the wiznet chips                                            |
//---------------------------------------------------------------------------------------+
#define NET_RST				BAND_GPIO_PA6 // Attached to both chips
#define NET_RDY_SERVER		BAND_GPIO_PC4 // Attached router (server)
#define NET_RDY_CLIENT		BAND_GPIO_PC5 // Attached computer (client)
#define NET_CS_SERVER		BAND_GPIO_PE1 // Attached router (server)
#define NET_CS_CLIENT		BAND_GPIO_PE2 // Attached computer (client)

//---------------------------------------------------------------------------------------+
// Register Offset Addresses                                                             |
//---------------------------------------------------------------------------------------+
#define NET_COMMON_MODE					(uint16)0x0000;
#define NET_COMMON_GATEWAY				(uint16)0x0001;
#define NET_COMMON_SUBNET				(uint16)0x0005;
#define NET_COMMON_MAC					(uint16)0x0009;
#define NET_COMMON_IP					(uint16)0x000F;
#define NET_COMMON_INTLEVEL				(uint16)0x0013;
#define NET_COMMON_IR					(uint16)0x0015;
#define NET_COMMON_IMR					(uint16)0x0016;
#define NET_COMMON_SIR					(uint16)0x0017;
#define NET_COMMON_SIMR					(uint16)0x0018;
#define NET_COMMON_VERSIONR				(uint16)0x0039;

#define NET_SOCKET_MR					(uint16)0x0000;
#define NET_SOCKET_CR					(uint16)0x0001;
#define NET_SOCKET_IR					(uint16)0x0002;
#define NET_SOCKET_SR					(uint16)0x0003;
#define NET_SOCKET_RXBUF_SIZE			(uint16)0x001E;
#define NET_SOCKET_TXBUF_SIZE			(uint16)0x001F;
#define NET_SOCKET_TX_FSR				(uint16)0x0020;
#define NET_SOCKET_TX_RD				(uint16)0x0022;
#define NET_SOCKET_TX_WR				(uint16)0x0024;
#define NET_SOCKET_RX_RSR				(uint16)0x0026;
#define NET_SOCKET_RX_RD				(uint16)0x0028;
#define NET_SOCKET_RX_WR				(uint16)0x002A;
#define NET_SOCKET_IMR					(uint16)0x002C;

#endif
