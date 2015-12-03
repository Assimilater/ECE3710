#include "../Shared/Controller.h"
#include "../Shared/GPIO.h"
#include "enet.h"


void NET_READDATA()
{
	
	//Address Phase
	SSI0->DR = 0x00;
	SSI0->DR = 0x00; //both bytes are specifying an address. Don't know what to do here yet
	
	//Control Phase  //Don't know what to do for BSB yet
	SSI0->DR = 0x00; //0bBSB[4:0]000 //last three bits says I'm reading and in Variable length Data mode
	
	//Data Phase
	//Read
}

void NET_PARSEDATA()
{
	
}

void NET_WRITEDATA()
{
	//Address Phase
	SSI0->DR = 0x00;
	SSI0->DR = 0x00; //both bytes are specifying an address. Don't know what to do here yet
	
	//Control Phase  //Don't know what to do for BSB yet
	SSI0->DR = 0x04; //0bBSB[4:0]100 //last three bits says I'm writing and in Variable length Data mode
	
	//Data Phase
	//Write
}
