#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"

void NET_WriteCmd (short address, char control)
{
	//Phase descriptions start at PG.18 in data sheet
	//Address Phase
	SSI0->DR = address >> 8 & 0xFF;
	SSI0->DR = address & 0xFF;
	//Control Phase  //BSB selects block //0bBSB[4:0]RWB[0]OM[1:0]
	SSI0->DR = control;
}

void NET_READDATA()
{
	NET_WriteCmd(0x0017,0x01); //Socket Interrput Register, read 1 byte
	
	//Data Phase
	SSI0->DR;
}

void NET_PARSEDATA()
{
	
}

void NET_WRITEDATA()
{
	NET_WriteCmd(0x0000,0x00);
	
	//Data Phase
	//Write
}
