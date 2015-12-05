#ifndef __WIZ550_H__
#define __WIZ550_H__

//---------------------------------------------------------------------------------------+
// Include Dependencies                                                                  |
//---------------------------------------------------------------------------------------+
#include "../Shared/embedded_t.h"

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
		uint socket:3; // Note: Must be 0 if using common register
		NET_REG reg:2;
		bool write:1; // Default: Read (0)
		NET_MODE mode:2; // Default: NET_VAR_M
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

//---------------------------------------------------------------------------------------+
// Driver Functions                                                                      |
//---------------------------------------------------------------------------------------+
void NET_WriteCmd(short, char);
void NET_READDATA(void);
void NET_PARSEDATA(void);
void NET_WRITEDATA(void);

bool NET_SPI_BYTE(NET_CHIP, NET_Byteframe*);
bool NET_SPI(NET_CHIP, NET_Frame*);
void NET_Init(void);

//---------------------------------------------------------------------------------------+
// Communication signals for the wiznet chips                                            |
//---------------------------------------------------------------------------------------+
#define NET_RST			BAND_GPIO_PA6 // Attached to both chips
#define NET_RDY_ISP		BAND_GPIO_PC4 // Internet Service Provider (server)
#define NET_RDY_CPC		BAND_GPIO_PC5 // Attached computer (client pc)
#define NET_CS_ISP		BAND_GPIO_PE1 // Internet Service Provider (server)
#define NET_CS_CPC		BAND_GPIO_PE2 // Attached computer (client pc)

//---------------------------------------------------------------------------------------+
// Register Offset Addresses                                                             |
//---------------------------------------------------------------------------------------+
#define NET_COMMON_MODE					(uint16)0x0000;
#define NET_COMMON_GATEWAY				(uint16)0x0001;
#define NET_COMMON_SUBN					(uint16)0x0005;
#define NET_COMMON_MAC					(uint16)0x0009;
#define NET_COMMON_IP					(uint16)0x000F;
#define NET_COMMON_IR					(uint16)0x0015;
#define NET_COMMON_IMR					(uint16)0x0016;
#define NET_COMMON_SIR					(uint16)0x0017;
#define NET_COMMON_SIMR					(uint16)0x0018;
#define NET_SOCKET_MODE					(uint16)0x0000;
#define NET_SOCKET_CR					(uint16)0x0001;
#define NET_SOCKET_IMR					(uint16)0x002C;

#endif
