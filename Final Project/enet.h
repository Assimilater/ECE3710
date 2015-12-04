#ifndef __WIZ550_H__
#define __WIZ550_H__

//---------------------------------------------------------------------------------------+
// Include Dependencies                                                                  |
//---------------------------------------------------------------------------------------+
#include "../Shared/embedded_t.h"
typedef unsigned short net_offset;

//---------------------------------------------------------------------------------------+
// Hepler Structs For Configuration                                                      |
//---------------------------------------------------------------------------------------+
typedef enum { NET_CHIP_CLIENT, NET_CHIP_SERVER } NET_CHIP_SELECT;
typedef enum { NET_COMMON_R, NET_SOCKET_R, NET_TX_R, NET_RX_R } NET_REGISTER;
typedef enum { NET_VAR_M, NET_F1B_M, NET_F2B_M, NET_F4B_M } NET_MODE;
typedef union {
	struct {
		int socket:3; // Note: Must be 0 if using common register
		NET_REGISTER reg:2;
		bool write:1; // Default: Read (0)
		NET_MODE op:2; // Default: NET_VAR_M
	};
	char byte;
} ControlByte; // See section 2.2.2 in w550 datasheet

typedef struct {
	net_offset Address;
	ControlByte Control;
	byte* Data;
	uint N;
} NET_Frame;

//---------------------------------------------------------------------------------------+
// Driver Functions                                                                      |
//---------------------------------------------------------------------------------------+
void NET_WriteCmd(short, char);
void NET_READDATA(void);
void NET_PARSEDATA(void);
void NET_WRITEDATA(void);

void NET_SPI(NET_CHIP_SELECT, NET_Frame*);
void NET_Init(void);

//---------------------------------------------------------------------------------------+
// Signal Definitions                                                                    |
//---------------------------------------------------------------------------------------+
#define NET_CS_ISP BAND_GPIO_PE1 // Internet Service Provider (server)
#define NET_CS_CPC BAND_GPIO_PE2 // Attached computer (client pc)

//---------------------------------------------------------------------------------------+
// Register Offset Addresses                                                             |
//---------------------------------------------------------------------------------------+
#define NET_COMMON_MODE					(net_offset)0x0000;
#define NET_SOCKET_MODE					(net_offset)0x0000;

#endif
